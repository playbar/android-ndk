#include "BrightnessFilter.h"

//#define __ARM_NEON__

#ifdef __ARM_NEON__

#include <cpu-features.h>
#include <arm_neon.h>

static void neonBrightnessFilter(
		unsigned short* pixels,
		long count,
		unsigned char brightness)
{
	const unsigned char MAX_RB = 0xF8;
	const unsigned char MAX_G = 0xFC;

	uint8x8_t maxRb = vmov_n_u8(MAX_RB);
	uint8x8_t maxG = vmov_n_u8(MAX_G);
	uint8x8_t increment = vmov_n_u8(brightness);

	for (long i = 0; i < count; i += 8)
	{
		// Load 8 16-bit pixels
		uint16x8_t rgb = vld1q_u16(&pixels[i]);

		// r = (pixels[i] >> 8) & MAX_RB;
		uint8x8_t r = vshrn_n_u16(rgb, 8);
		r = vand_u8(r, maxRb);

		// g = (pixels[i] >> 3) & MAX_G;
		uint8x8_t g = vshrn_n_u16(rgb, 3);
		g = vand_u8(g, maxG);

		// b = (pixels[i] << 3) & MAX_RB;
		uint8x8_t b = vmovn_u16(rgb);
		b = vshl_n_u8(b, 3);
		b = vand_u8(b, maxRb);

		// r += brightness;
		r = vadd_u8(r, increment);

		// g += brightness;
		g = vadd_u8(g, increment);

		// b += brightness;
		b = vadd_u8(b, increment);

		// r = (r > MAX_RB) ? MAX_RB : r;
		r = vmin_u8(r, maxRb);

		// g = (g > MAX_G) ? MAX_G : g;
		g = vmin_u8(g, maxG);

		// b = (b > MAX_RB) ? MAX_RB : b;
		b = vmin_u8(b, maxRb);

		// pixels[i] = (r << 8);
		rgb = vshll_n_u8(r, 8);

		// pixels[i] |= (g << 3);
		uint16x8_t g16 = vshll_n_u8(g, 8);
		rgb = vsriq_n_u16(rgb, g16, 5);

		// pixels[i] |= (b >> 3);
		uint16x8_t b16 = vshll_n_u8(b, 8);
		rgb = vsriq_n_u16(rgb, b16, 11);

		// Store 8 16-bit pixels
		vst1q_u16(&pixels[i], rgb);
	}
}

#endif

static void genericBrightnessFilter(
		unsigned short* pixels,
		long count,
		unsigned char brightness)
{
	const unsigned char MAX_RB = 0xF8;
	const unsigned char MAX_G = 0xFC;

	unsigned short r, g, b;

	for (long i = 0; i < count; i++)
	{
		// Decompose colors
		r = (pixels[i] >> 8) & MAX_RB;
		g = (pixels[i] >> 3) & MAX_G;
		b = (pixels[i] << 3) & MAX_RB;

		// Brightness increment
		r += brightness;
		g += brightness;
		b += brightness;

		// Make sure that components are in range
		r = (r > MAX_RB) ? MAX_RB : r;
		g = (g > MAX_G) ? MAX_G : g;
		b = (b > MAX_RB) ? MAX_RB : b;

		// Set pixel
		pixels[i] = (r << 8);
		pixels[i] |= (g << 3);
		pixels[i] |= (b >> 3);
	}
}

void brightnessFilter(
		unsigned short* pixels,
		long count,
		unsigned char brightness)
{
#ifdef __ARM_NEON__

	// Get the CPU family
	AndroidCpuFamily cpuFamily = android_getCpuFamily();

	// Get the CPU features
	uint64_t cpuFeatures = android_getCpuFeatures();

	// Use NEON optimized function only on ARM CPUs with NEON support
	if ((ANDROID_CPU_FAMILY_ARM == cpuFamily)
			&& ((ANDROID_CPU_ARM_FEATURE_NEON & cpuFeatures) != 0))
	{
		// Invoke the NEON optimized brightness filter
		neonBrightnessFilter(pixels, count, brightness);
	}
	else
	{
#endif
		// Invoke the generic brightness filter
		genericBrightnessFilter(pixels, count, brightness);
#ifdef __ARM_NEON__
	}
#endif
}
