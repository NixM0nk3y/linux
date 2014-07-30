/*
 * Driver for the PCM1803A codec
 *
 * Author:      Nick Gregory <nick@openenterprise.co.uk>
 *              Copyright 2014
 *
 * based on pcm1794a.c
 * Author:	Florian Meier <florian.meier@koalo.de>
 *		Copyright 2013
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/soc.h>

#include "pcm1803a.h"

static int pcm1803a_hw_params(struct snd_pcm_substream *substream,
struct snd_pcm_hw_params *params,
struct snd_soc_dai *dai)
{
        return 0;
}

static int pcm1803a_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
        return 0;
}

static int pcm1803a_set_clkdiv(struct snd_soc_dai *dai,
int div_id, int div)
{
        return 0;
}

static int pcm1803a_set_sysclk(struct snd_soc_dai *dai,
int clk_id, unsigned int freq, int dir)
{
        return 0;
}

static int pcm1803a_set_pll(struct snd_soc_dai *dai, int pll_id,
int source, unsigned int freq_in,
unsigned int freq_out)
{
        return 0;
}

static const struct snd_soc_dai_ops pcm1803a_dai_ops = {
        .hw_params  = pcm1803a_hw_params,
        .set_fmt    = pcm1803a_set_fmt,
        .set_sysclk = pcm1803a_set_sysclk,
        .set_clkdiv = pcm1803a_set_clkdiv,
        .set_pll    = pcm1803a_set_pll
};

static struct snd_soc_dai_driver pcm1803a_dai = {
	.name = "pcm1803a-hifi",
	.capture = {
                .stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
                .rates = SNDRV_PCM_RATE_48000,
                .formats = SNDRV_PCM_FMTBIT_S32_LE
	},
        .ops = &pcm1803a_dai_ops,
};

static struct snd_soc_codec_driver soc_codec_dev_pcm1803a;

static int pcm1803a_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_pcm1803a,
			&pcm1803a_dai, 1);
}

static int pcm1803a_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver pcm1803a_codec_driver = {
	.probe 		= pcm1803a_probe,
	.remove 	= pcm1803a_remove,
	.driver		= {
		.name	= "pcm1803a-codec",
		.owner	= THIS_MODULE,
	},
};

module_platform_driver(pcm1803a_codec_driver);

MODULE_DESCRIPTION("ASoC PCM1803A codec driver");
MODULE_AUTHOR("Nick Gregory <nick@openenterprise.co.uk>");
MODULE_LICENSE("GPL v2");
