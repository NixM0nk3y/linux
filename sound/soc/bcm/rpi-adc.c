/*
 * ASoC Driver for RPi-ADC.
 *
 * Author:	Nick Gregory <nick@openenterprise.co.uk>
 *		Copyright 2014
 *
 * based on rpi-dac.c 
 * Author:      Florian Meier <florian.meier@koalo.de>
 *              Copyright 2013
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

#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

#include "../codecs/pcm1803a.h"

static int samplerate = 44100;

static int snd_rpi_rpi_adc_init(struct snd_soc_pcm_runtime *rtd)
{
	return 0;
}

static int snd_rpi_rpi_adc_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
        struct snd_soc_dai *codec_dai = rtd->codec_dai;

        int sysclk =  24576000 ; /* This is fixed on this board */

        int ret;

        samplerate = params_rate(params);
      
        ret = snd_soc_dai_set_pll(codec_dai, PCM1803A_PLL1, PCM1803A_PLL_SRC_MCLK1, samplerate * 512, samplerate * 64);

        if (ret < 0) {
                dev_err(substream->pcm->dev,
                "Failed to set ADC PLL: %d\n", ret);
                return ret;
        }

        ret = snd_soc_dai_set_sysclk(codec_dai, PCM1803A_CLK_SRC_PLL1, samplerate * 512 , SND_SOC_CLOCK_IN);

        if (ret < 0) {
                dev_err(substream->pcm->dev,
                "Failed to set ADC SYSCLK: %d\n", ret);
                return ret;
        }

        //return snd_soc_dai_set_bclk_ratio(cpu_dai, 32*2);
        return 0;
}

/* machine stream operations */
static struct snd_soc_ops snd_rpi_rpi_adc_ops = {
	.hw_params = snd_rpi_rpi_adc_hw_params,
};

static struct snd_soc_dai_link snd_rpi_rpi_adc_dai[] = {
{
	.name		= "PCM1804A ADC",
	.stream_name	= "PCM1804A ADC HiFi",
	.cpu_dai_name	= "bcm2708-i2s.0",
	.codec_dai_name	= "pcm1803a-hifi",
	.platform_name	= "bcm2708-i2s.0",
	.codec_name	= "pcm1803a-codec",
	.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				SND_SOC_DAIFMT_CBM_CFM,
	.ops		= &snd_rpi_rpi_adc_ops,
	.init		= snd_rpi_rpi_adc_init,
},
};

/* audio machine driver */
static struct snd_soc_card snd_rpi_rpi_adc = {
	.name         = "snd_rpi_rpi_adc",
	.dai_link     = snd_rpi_rpi_adc_dai,
	.num_links    = ARRAY_SIZE(snd_rpi_rpi_adc_dai),
};

static int snd_rpi_rpi_adc_probe(struct platform_device *pdev)
{
	int ret = 0;

	snd_rpi_rpi_adc.dev = &pdev->dev;
	ret = snd_soc_register_card(&snd_rpi_rpi_adc);
	if (ret)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);

	return ret;
}

static int snd_rpi_rpi_adc_remove(struct platform_device *pdev)
{
	return snd_soc_unregister_card(&snd_rpi_rpi_adc);
}

static struct platform_driver snd_rpi_rpi_adc_driver = {
        .driver = {
                .name   = "snd-rpi-adc",
                .owner  = THIS_MODULE,
        },
        .probe          = snd_rpi_rpi_adc_probe,
        .remove         = snd_rpi_rpi_adc_remove,
};

module_platform_driver(snd_rpi_rpi_adc_driver);

MODULE_AUTHOR("Nick Gregory <nick@openenterprise.co.uk>");
MODULE_DESCRIPTION("ASoC Driver for PCM1804A ADC");
MODULE_LICENSE("GPL v2");
