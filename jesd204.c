
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "jesd204-topo.h"

#define ARRAY_SIZE(x) \
	(sizeof(x) / sizeof((x)[0]))

enum jesd204_state_op_reason {
	JESD204_STATE_OP_REASON_INIT,
	JESD204_STATE_OP_REASON_UNINIT,
};

enum jesd204_dev_op {
	JESD204_OP_DEVICE_INIT,
	JESD204_OP_LINK_INIT,
	JESD204_OP_LINK_SUPPORTED,
	JESD204_OP_LINK_PRE_SETUP,
	JESD204_OP_CLK_SYNC_STAGE1,
	JESD204_OP_CLK_SYNC_STAGE2,
	JESD204_OP_CLK_SYNC_STAGE3,
	JESD204_OP_LINK_SETUP,
	JESD204_OP_OPT_SETUP_STAGE1,
	JESD204_OP_OPT_SETUP_STAGE2,
	JESD204_OP_OPT_SETUP_STAGE3,
	JESD204_OP_OPT_SETUP_STAGE4,
	JESD204_OP_OPT_SETUP_STAGE5,
	JESD204_OP_CLOCKS_ENABLE,
	JESD204_OP_LINK_ENABLE,
	JESD204_OP_LINK_RUNNING,
	JESD204_OP_OPT_POST_RUNNING_STAGE,

	__JESD204_MAX_OPS,
};

enum jesd204_state_op_mode {
	JESD204_STATE_OP_MODE_PER_LINK,
	JESD204_STATE_OP_MODE_PER_DEVICE,
};

typedef int (*jesd204_dev_cb)(struct jesd204_dev *jdev,
			      enum jesd204_state_op_reason reason);

typedef int (*jesd204_link_cb)(struct jesd204_dev *jdev,
			       enum jesd204_state_op_reason,
			       struct jesd204_link *lnk);

struct jesd204_state_op {
	enum jesd204_state_op_mode	mode;
	jesd204_dev_cb			per_device;
	jesd204_link_cb			per_link;
	bool				post_state_sysref;
};

struct jesd204_dev_data {
	size_t					sizeof_priv;
	unsigned int				max_num_links;
	unsigned int				num_retries;
	struct jesd204_state_op			state_ops[__JESD204_MAX_OPS];
};

enum jesd204_state_change_result {
	JESD204_STATE_CHANGE_ERROR = -1,
	JESD204_STATE_CHANGE_DEFER = 0,
	JESD204_STATE_CHANGE_DONE,
};

int ad9081_jesd204_uninit(struct jesd204_dev *jdev,
			    enum jesd204_state_op_reason reason)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_link_init(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_clks_enable(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_link_enable(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_link_running(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_setup_stage1(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_setup_stage2(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

static int ad9081_jesd204_setup_stage3(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf("%s: %s %s\n", jdev->name, __FUNCTION__, !reason ? "init" : "uninit");

	return 0;
}

struct ad9081_jesd204_priv {
};

static const struct jesd204_dev_data jesd204_ad9081_init = {
	.state_ops = {
		[JESD204_OP_DEVICE_INIT] = {
			.per_device = ad9081_jesd204_uninit,
		},
		[JESD204_OP_LINK_INIT] = {
			.per_link = ad9081_jesd204_link_init,
		},
		[JESD204_OP_CLOCKS_ENABLE] = {
			.per_link = ad9081_jesd204_clks_enable,
		},
		[JESD204_OP_LINK_ENABLE] = {
			.per_link = ad9081_jesd204_link_enable,
		},
		[JESD204_OP_LINK_RUNNING] = {
			.per_link = ad9081_jesd204_link_running,
		},
		[JESD204_OP_OPT_SETUP_STAGE1] = {
			.per_device = ad9081_jesd204_setup_stage1,
			.mode = JESD204_STATE_OP_MODE_PER_DEVICE,
		},
		[JESD204_OP_OPT_SETUP_STAGE2] = {
			.per_device = ad9081_jesd204_setup_stage2,
			.mode = JESD204_STATE_OP_MODE_PER_DEVICE,
		},
		[JESD204_OP_OPT_SETUP_STAGE3] = {
			.per_device = ad9081_jesd204_setup_stage3,
			.mode = JESD204_STATE_OP_MODE_PER_DEVICE,
		},
	},

	.max_num_links = 2,
	.num_retries = 3,
	.sizeof_priv = sizeof(struct ad9081_jesd204_priv),
};

typedef int (*jesd204_dev_cb)(struct jesd204_dev *jdev,
			      enum jesd204_state_op_reason reason);

typedef int (*jesd204_link_cb)(struct jesd204_dev *jdev,
			       enum jesd204_state_op_reason,
			       struct jesd204_link *lnk);

struct jesd204_init_arg {
	const struct jesd204_dev_data *data;
	enum jesd204_dev_op op;
	enum jesd204_state_op_reason reason;
};

static int _xinit(struct jesd204_dev *jdev, void *arg)
{
	struct jesd204_init_arg *iarg = (struct jesd204_init_arg *)arg;
	enum jesd204_dev_op op = iarg->op;
	enum jesd204_state_op_reason reason = iarg->reason;

	if (iarg->data->state_ops[op].per_device)
		return iarg->data->state_ops[op].per_device(jdev, reason);
	return 0;
}

int jesd204_init(struct jesd204_dev *jdev, const struct jesd204_dev_data *init_data) {
	int ret;
	enum jesd204_dev_op op;
	struct jesd204_init_arg arg;

	if (!init_data)
		return -EINVAL;

	arg.data = init_data;
	arg.reason = JESD204_STATE_OP_REASON_INIT;

	// TODO: add retry mechanism based on init_data->num_retries

	for (op = 0; op < __JESD204_MAX_OPS; op++) {
		arg.op = op;
		ret = jtopo_for_all(jdev, _xinit, (void *)&arg);
		if (ret < 0)
			goto uninit;
	}
	return ret;
uninit:
	arg.reason = JESD204_STATE_OP_REASON_UNINIT;
	for (; op > 0; op--) {
		arg.op = op;
		// Rolling back doesn't stop even when any of the states errors out;
		// it is of higher priority to reach back to IDLE state, than to stop when rolling back.
		// Therefore, ignore the error code.
		jtopo_for_all(jdev, _xinit, (void *)&arg);
	}
	return 0;
}

int main(void)
{
	struct jesd204_dev *jdev;
	struct jesd204_dev_info jdev_info;
	// Top-level device has no output, hence we call it with NULL.
	jdev = jtopo_device("trx0_ad9081", NULL, &jdev_info);
	if (jdev == NULL)
		goto error;

	struct jesd204_dev *axi_ad9081_core_rx;
	struct jesd204_dev_info axi_ad9081_core_rx_info;
	axi_ad9081_core_rx = jtopo_device("axi_ad9081_core_rx", jdev, &axi_ad9081_core_rx_info);
	if (axi_ad9081_core_rx == NULL)
		goto error;

	/*
	// 2 Dummy devices under axi_ad9081_core_rx 
	struct jesd204_dev *dummy;
	struct jesd204_dev_info dummy_info;
	dummy = jtopo_device("dummy", axi_ad9081_core_rx, &dummy_info);
	if (dummy == NULL)
		goto error;
	struct jesd204_dev *dummy2;
	struct jesd204_dev_info dummy2_info;
	dummy2 = jtopo_device("dummy2", axi_ad9081_core_rx, &dummy2_info);
	if (dummy2 == NULL)
		goto error;
	*/

	struct jesd204_dev *axi_ad9081_core_tx;
	struct jesd204_dev_info axi_ad9081_core_tx_info;
	axi_ad9081_core_tx = jtopo_device("axi_ad9081_core_tx", jdev, &axi_ad9081_core_tx_info);
	if (axi_ad9081_core_tx == NULL)
		goto error;

	struct jesd204_dev *hmc7044;
	struct jesd204_dev_info hmc7044_info;
	hmc7044 = jtopo_device("hmc7044", axi_ad9081_core_rx, &hmc7044_info);
	if (hmc7044 == NULL)
		goto error;
	
	jtopo_connect(hmc7044, axi_ad9081_core_tx);

	jesd204_init(jdev, &jesd204_ad9081_init);
	
	jtopo_delete(jdev);
error:
	printf("Bye\n");

	return 0;
}
