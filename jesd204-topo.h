#include <stdint.h>
#include <stdbool.h>

struct jesd204_dev_info {
	bool sysref_provider;
};

struct jesd204_dev {
	uint32_t id;
	const char *name;
	struct jesd204_dev **inputs;
	uint16_t inputs_count;
	struct jesd204_dev *output;
	struct jesd204_dev_info *info;
};

struct jesd204_link {
};

typedef int (*jtopo_iter_cb)(struct jesd204_dev *dev, void *arg);

struct jesd204_dev * jtopo_device(const char *name, struct jesd204_dev *output,
		     struct jesd204_dev_info *info);
int jtopo_for_all(struct jesd204_dev *dev, jtopo_iter_cb callback, void *arg);
void jtopo_delete(struct jesd204_dev *dev);