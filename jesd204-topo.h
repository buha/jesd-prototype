#include <stdint.h>
#include <stdbool.h>

struct jesd204_dev_info {
	bool sysref_provider;
};

enum jesd204_sysref_mode {
	JESD204_SYSREF_DISABLED,
	JESD204_SYSREF_CONTINUOUS,
	JESD204_SYSREF_ONESHOT,
};

struct jesd204_sysref {
	enum jesd204_sysref_mode	mode;
	uint8_t				capture_falling_edge;
	uint8_t				valid_falling_edge;
	uint16_t			lmfc_offset;
};

struct jesd204_link {
	uint64_t sample_rate;
	uint32_t sample_rate_div;

	bool is_transmit;

	uint8_t num_lanes;
	uint8_t num_converters;
	uint8_t octets_per_frame;
	uint16_t frames_per_multiframe;
	uint8_t num_of_multiblocks_in_emb; /* E */

	uint8_t bits_per_sample;

	uint8_t converter_resolution;
	uint8_t jesd_version;
	uint8_t jesd_encoder;
	uint8_t subclass;

	uint8_t device_id;
	uint8_t bank_id;

	uint8_t scrambling;
	uint8_t high_density;

	uint8_t ctrl_words_per_frame_clk;
	uint8_t ctrl_bits_per_sample;
	uint8_t samples_per_conv_frame;

	uint8_t *lane_ids;

	struct jesd204_sysref sysref;

	/* Subclass 2 only */
	uint8_t dac_adj_resolution_steps;
	uint8_t dac_adj_direction;
	uint8_t dac_phase_adj;
};

struct jesd204_dev {
	uint32_t id;
	const char *name;
	struct jesd204_dev **inputs;
	uint16_t inputs_count;
	struct jesd204_dev **outputs;
	uint16_t outputs_count;
	struct jesd204_link **links;
	uint16_t links_count;
	struct jesd204_dev_info *info;
};

typedef int (*jtopo_iter_cb)(struct jesd204_dev *dev, void *arg);

struct jesd204_dev * jtopo_device(const char *name, struct jesd204_dev *output,
		     struct jesd204_link *link, struct jesd204_dev_info *info);
int jtopo_connect(struct jesd204_dev *out, struct jesd204_dev *in, struct jesd204_link *link);
int jtopo_for_all(struct jesd204_dev *dev, jtopo_iter_cb callback, void *arg);
unsigned int jtopo_count(struct jesd204_dev *dev);
void jtopo_delete(struct jesd204_dev *dev);