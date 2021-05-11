#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "jesd204-topo.h"

static struct jesd204_dev * _jtopo_top_device(struct jesd204_dev *dev)
{
	struct jesd204_dev *devi = dev;

	if (!dev)
		return NULL;

	while(devi->outputs_count)
		devi = devi->outputs[0];
	
	return devi;
}

struct jesd204_dev * jtopo_device(const char *name, struct jesd204_dev *output,
		     struct jesd204_dev_info *info)
{
	struct jesd204_dev *dev;
	static uint32_t id = 0;
	
	if (!info)
		return NULL;

	dev = calloc(1, sizeof(struct jesd204_dev));
	if (!dev)
		return NULL;

	dev->outputs = calloc(1, sizeof(struct jesd204_dev *));
	if (!dev->outputs)
		goto error1;

	dev->name = name;
	dev->info = info;
	dev->id = id++;
	dev->outputs[0] = output;

	if (output) {
		uint32_t new_inputs_count = output->inputs_count + 1;
		struct jesd204_dev **inputs = realloc(output->inputs,
			new_inputs_count * sizeof(struct jesd204_dev *));
		if (!inputs)
			goto error2;
		output->inputs = inputs;
		output->inputs[output->inputs_count] = dev;
		output->inputs_count = new_inputs_count;
	}

	return dev;
error2:
	free(dev->outputs);
error1:
	free(dev);
	return NULL;
}

int jtopo_connect(struct jesd204_dev *out, struct jesd204_dev *in)
{
	int ret;
	uint32_t new_outputs_count, new_inputs_count;

	if (!out || !in)
		return -EINVAL;
	
	new_outputs_count = out->outputs_count + 1;
	struct jesd204_dev **outputs = realloc(out->outputs,
		new_outputs_count * sizeof(struct jesd204_dev *));
	if (!outputs)
		return -ENOMEM;

	new_inputs_count = in->inputs_count + 1;
	struct jesd204_dev **inputs = realloc(in->inputs,
		new_inputs_count * sizeof(struct jesd204_dev *));
	if (!inputs) {
		ret = -ENOMEM;
		goto error;
	}

	out->outputs = outputs;
	out->outputs[out->outputs_count] = in;
	out->outputs_count = new_outputs_count;

	in->inputs = inputs;
	in->inputs[in->inputs_count] = out;
	in->inputs_count = new_inputs_count;

	return 0;
error:
	free(outputs);
	return ret;
}

int jtopo_for_all(struct jesd204_dev *dev, jtopo_iter_cb callback, void *arg)
{
	int ret;
	uint16_t i;

	if (!dev || !callback)
		return 0;

	if (dev->inputs_count)
		for (i = 0; i < dev->inputs_count; i++) {
			ret = jtopo_for_all(dev->inputs[i], callback, arg);
			if (ret < 0)
				return ret;
		}

	ret = callback(dev, arg);

	return ret;
}

static int _delete_dev(struct jesd204_dev *dev, void *arg)
{
	unsigned int o, i;
	printf("%s: %s id:%d\n", __FUNCTION__, dev->name, dev->id);
	if (dev->inputs)
		free(dev->inputs);
	
	// We go up the tree.
	// References to dev higher in the tree should reflect that the memory was freed.
	for(o = 0; o < dev->outputs_count; o++)
	 	for(i = 0; i < dev->outputs[o]->inputs_count; i++) {
			 if (dev->outputs[o]->inputs[i]->id == dev->id)
				dev->outputs[o]->inputs[i] = NULL;
		}

	free(dev);
	return 0;
}

void jtopo_delete(struct jesd204_dev *dev)
{
	struct jesd204_dev *top = _jtopo_top_device(dev);
	jtopo_for_all(top, _delete_dev, NULL);
}