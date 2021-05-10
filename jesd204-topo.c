#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "jesd204-topo.h"

static struct jesd204_dev * _jtopo_top_device(struct jesd204_dev *dev)
{
	struct jesd204_dev *devi = dev;

	if (!dev)
		return NULL;

	while(devi->output)
		devi = devi->output;
	
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

	dev->name = name;
	dev->output = output;
	dev->info = info;
	dev->id = id++;

	if (dev->output) {
		uint32_t new_inputs_count = dev->output->inputs_count + 1;
		struct jesd204_dev **inputs = realloc(dev->output->inputs,
			new_inputs_count * sizeof(struct jesd204_dev));
		if (!inputs)
			goto error;
		dev->output->inputs = inputs;
		dev->output->inputs[dev->output->inputs_count] = dev;
		dev->output->inputs_count = new_inputs_count;
	}

	return dev;
error:
	free(dev);
	return NULL;
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
	printf("%s: %s id:%d\n", __FUNCTION__, dev->name, dev->id);
	if (dev->inputs)
		free(dev->inputs);
	free(dev);
	return 0;
}

void jtopo_delete(struct jesd204_dev *dev)
{
	struct jesd204_dev *top = _jtopo_top_device(dev);
	jtopo_for_all(top, _delete_dev, NULL);
}