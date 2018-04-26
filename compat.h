#ifndef _COMPAT_H_
#define _COMPAT_H_

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#else
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))

#ifndef NETIF_F_HW_VLAN_CTAG_RX
#define NETIF_F_HW_VLAN_CTAG_RX NETIF_F_HW_VLAN_RX
#endif

#ifndef NETIF_F_HW_VLAN_CTAG_TX
#define NETIF_F_HW_VLAN_CTAG_TX NETIF_F_HW_VLAN_TX
#endif

#ifndef NETIF_F_HW_VLAN_CTAG_FILTER
#define NETIF_F_HW_VLAN_CTAG_FILTER NETIF_F_HW_VLAN_FILTER
#endif

static inline struct sk_buff *__kc__vlan_hwaccel_put_tag(struct sk_buff *skb,
							 u16 vlan_tci)
{
#ifdef VLAN_TAG_PRESENT
	vlan_tci |= VLAN_TAG_PRESENT;
#endif
	skb->vlan_tci = vlan_tci;
        return skb;
}
#define __vlan_hwaccel_put_tag(skb, vlan_proto, vlan_tci) \
	__kc__vlan_hwaccel_put_tag(skb, vlan_tci)

#endif /*3.10.0*/

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 13, 0) )

/*SmPL*/
#define dma_set_mask_and_coherent(_p, _m) __kc_dma_set_mask_and_coherent(_p, _m)
extern int __kc_dma_set_mask_and_coherent(struct device *dev, u64 mask);

#endif

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0) )

#ifndef PCI_VENDOR_ID_VMWARE
#define PCI_VENDOR_ID_VMWARE 0x15ad
#endif

void e1000_free_frag(const void *data);

#endif

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 2, 0))

#ifndef skb_free_frag
#define skb_free_frag(data) e1000_free_frag(data)
#endif

#endif

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0))

#if !defined(eth_skb_pad) && !defined(skb_put_padto)
static inline int __kc_skb_put_padto(struct sk_buff *skb, unsigned int len)
{
	unsigned int size = skb->len;

	if (unlikely(size < len)) {
		len -= size;
		if (skb_pad(skb, len))
			return -ENOMEM;
		__skb_put(skb, len);
	}
	return 0;
}
#define skb_put_padto(skb, len) __kc_skb_put_padto(skb, len)

static inline int __kc_eth_skb_pad(struct sk_buff *skb)
{
	return __kc_skb_put_padto(skb, ETH_ZLEN);
}
#define eth_skb_pad(skb) __kc_eth_skb_pad(skb)
#endif /* eth_skb_pad && skb_put_padto */

static inline void _kc_napi_complete_done(struct napi_struct *napi,
					  int __always_unused work_done)
{
	napi_complete(napi);
}
#define napi_complete_done _kc_napi_complete_done

#ifndef dma_rmb
#define dma_rmb() rmb()
#endif

#ifndef SKB_ALLOC_NAPI
static inline struct sk_buff *__kc_napi_alloc_skb(struct napi_struct *napi,
						  unsigned int length)
{
	return netdev_alloc_skb_ip_align(napi->dev, length);
}

#define napi_alloc_skb(napi,len) __kc_napi_alloc_skb(napi,len)
#endif /* SKB_ALLOC_NAPI */


#endif /* 3.19.0 */

/***********************************************************/
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 20, 0))

/*SmPL,here we use Spatch instead of marcos*/

#ifndef skb_vlan_tag_get
#define skb_vlan_tag_get vlan_tx_tag_get
#endif

#ifndef skb_vlan_tag_present
#define skb_vlan_tag_present vlan_tx_tag_present
#endif

#else
#define HAVE_INCLUDE_LINUX_TIMECOUNTER_H
#define HAVE_NDO_BRIDGE_SET_DEL_LINK_FLAGS

#endif /*3.20.0*/

#endif /*_COMPAT_H_*/
