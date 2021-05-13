
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "jesd204-topo.h"

const char pretty[] = "%-20s: %-10s %-20s() %-10s\n";

int ad9081_jesd204_uninit(struct jesd204_dev *jdev,
			    enum jesd204_state_op_reason reason)
{
	printf(pretty, jdev->name, "", __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_link_init(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf(pretty, jdev->name, lnk->name, __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_clks_enable(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf(pretty, jdev->name, lnk->name, __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_link_enable(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf(pretty, jdev->name, lnk->name, __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_link_running(struct jesd204_dev *jdev,
		enum jesd204_state_op_reason reason,
		struct jesd204_link *lnk)
{
	printf(pretty, jdev->name, lnk->name, __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_setup_stage1(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf(pretty, jdev->name, "", __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_setup_stage2(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf(pretty, jdev->name, "", __FUNCTION__, !reason ? "" : "UNINIT");

	return 0;
}

static int ad9081_jesd204_setup_stage3(struct jesd204_dev *jdev,
					 enum jesd204_state_op_reason reason)
{
	printf(pretty, jdev->name, "", __FUNCTION__, !reason ? "" : "UNINIT");

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

int main(void)
{
	int ret;
	struct jesd204_dev *jdev;
	struct jesd204_dev_info jdev_info;
	struct jesd204_link *rx;
	struct jesd204_link *tx;
	struct jesd204_link_param rx_link_param = {
		// TODO: fill out - this is equivalent to link configuration in linux device tree
	};
	struct jesd204_link_param tx_link_param = {
		// TODO: fill out - this is equivalent to link configuration in linux device tree
	};

	ret = jtopo_link("rx_link", &rx, rx_link_param);
	if (ret < 0)
		goto error;
	ret = jtopo_link("tx_link", &tx, tx_link_param);
	if (ret < 0)
		goto error;

	// Top-level device has no output, hence we call it with NULL.
	jdev = jtopo_device("trx0_ad9081", NULL, NULL, &jdev_info);
	if (jdev == NULL)
		goto error;

	struct jesd204_dev *axi_ad9081_core_rx;
	struct jesd204_dev_info axi_ad9081_core_rx_info;
	axi_ad9081_core_rx = jtopo_device("axi_ad9081_core_rx", jdev,  rx, &axi_ad9081_core_rx_info);
	if (axi_ad9081_core_rx == NULL)
		goto error;

	struct jesd204_dev *axi_ad9081_core_tx;
	struct jesd204_dev_info axi_ad9081_core_tx_info;
	axi_ad9081_core_tx = jtopo_device("axi_ad9081_core_tx", jdev, tx, &axi_ad9081_core_tx_info);
	if (axi_ad9081_core_tx == NULL)
		goto error;

	struct jesd204_dev *hmc7044;
	struct jesd204_dev_info hmc7044_info;
	hmc7044 = jtopo_device("hmc7044", axi_ad9081_core_rx, rx, &hmc7044_info);
	if (hmc7044 == NULL)
		goto error;
	
	jtopo_connect(hmc7044, axi_ad9081_core_tx, tx);

	jesd204_init(jdev, &jesd204_ad9081_init);
	
	jtopo_delete(jdev);
error:
	printf("Bye\n");

	return 0;
}
