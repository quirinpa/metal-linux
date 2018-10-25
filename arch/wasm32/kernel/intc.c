/*
 * Copyright (C) 2011-12 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/irqdomain.h>
#include <linux/irqchip.h>
#include <asm/irq.h>
#include <linux/printk.h>

/*
 * Early Hardware specific Interrupt setup
 * -Platform independent, needed for each CPU (not foldable into init_IRQ)
 * -Called very early (start_kernel -> setup_arch -> setup_processor)
 *
 * what it does ?
 * -Optionally, setup the High priority Interrupts as Level 2 IRQs
 */
void arc_init_IRQ(void)
{
	pr_err("%s: Implement me.\n", __func__);
}

/*
 * ARC700 core includes a simple on-chip intc supporting
 * -per IRQ enable/disable
 * -2 levels of interrupts (high/low)
 * -all interrupts being level triggered
 *
 * To reduce platform code, we assume all IRQs directly hooked-up into intc.
 * Platforms with external intc, hence cascaded IRQs, are free to over-ride
 * below, per IRQ.
 */

static void arc_irq_mask(struct irq_data *data)
{
	panic("%s: Not implemented\n", __func__);
}

static void arc_irq_unmask(struct irq_data *data)
{
	panic("%s: Not implemented\n", __func__);
}

static struct irq_chip onchip_intc = {
	.name           = "ARC In-core Intc",
	.irq_mask	= arc_irq_mask,
	.irq_unmask	= arc_irq_unmask,
};

static int __init
init_onchip_IRQ(struct device_node *intc, struct device_node *parent)
{
	panic("%s: Not implemented\n", __func__);
	return 0;
}

IRQCHIP_DECLARE(arc_intc, "snps,arc700-intc", init_onchip_IRQ);
