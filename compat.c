#include "e1000.h"
#include "compat.h"

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 13, 0))

int __kc_dma_set_mask_and_coherent(struct device *dev, u64 mask)
{
	int err = dma_set_mask(dev, mask);

	if (!err)
		/* coherent mask for the same size will always succeed if
		 * dma_set_mask does. However we store the error anyways, due
		 * to some kernels which use gcc's warn_unused_result on their
		 * definition of dma_set_coherent_mask.
		 */
		err = dma_set_coherent_mask(dev, mask);
	return err;
}
#endif

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0) )

void e1000_free_frag(const void *data)
{
	put_page(virt_to_head_page(data));
}

#endif

