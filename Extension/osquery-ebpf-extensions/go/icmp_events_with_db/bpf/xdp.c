//go:build ignore

#include "bpf_endian.h"
#include "common.h"

char __license[] SEC("license") = "Dual MIT/GPL";

#define MAX_MAP_ENTRIES 16

struct icmp_info {
	__u32 ip;
	__be16 seq_no;
	__u8 type;
	__u8 ttl;
};

struct icmphdr {
  __u8		type;
  __u8		code;
  __sum16	checksum;
  union {
	struct {
		__be16	id;
		__be16	sequence;
	} echo;
	__be32	gateway;
	struct {
		__be16	__unused;
		__be16	mtu;
	} frag;
	__u8	reserved[4];
  } un;
};

/* Define an Array map for storing ICMP packets info */
struct {
	__uint(type, BPF_MAP_TYPE_QUEUE);
	__uint(max_entries, MAX_MAP_ENTRIES);
	__type(value, struct icmp_info); // ICMP info
} xdp_stats_map SEC(".maps");

/*
Attempt to parse ICMP info from the packet.
Returns 0 if there is no IPv4 header field.
Returns 0 if the IP protocol is not ICMP.
Otherwise returns non-zero.
*/
static __always_inline int parse_icmp(struct xdp_md *ctx, struct icmp_info *icmpinfo) {
	void *data_end = (void *)(long)ctx->data_end;
	void *data     = (void *)(long)ctx->data;

	// First, parse the ethernet header.
	struct ethhdr *eth = data;
	if ((void *)(eth + 1) > data_end) {
		return 0;
	}

	if (eth->h_proto != bpf_htons(ETH_P_IP)) {
		// The protocol is not IPv4, so we can't parse an IPv4 source address.
		return 0;
	}

	// Then parse the IP header.
	struct iphdr *ip = (void *)(eth + 1);
	if ((void *)(ip + 1) > data_end) {
		return 0;
	}

	// Return 0 if the packet is not ICMP
	if (ip->protocol != 1) {
		return 0;
	}

	// Get the source IP address in network byte order.
	icmpinfo->ip = (__u32)(ip->saddr);
	// Get the ttl value of the packet
	icmpinfo->ttl = (__u8)(ip->ttl);

	// Parse the ICMP header
	struct icmphdr *icmp = (void *)(ip + 1);
	if ((void *)(icmp + 1) > data_end) {
		return 0;
	}

	// Get the ICMP sequence number and message type
	icmpinfo->seq_no = (__be16)(icmp->un.echo.sequence);
	icmpinfo->type = (__u8)(icmp->type);
	
	return 1;
}

SEC("xdp")
int xdp_prog_func(struct xdp_md *ctx) {
	struct icmp_info icmpinfo;
	int p = parse_icmp(ctx, &icmpinfo);
	if (!p) {
		// Not an IPv4 ICMP packet, so don't count it.
		goto done;
	}

	bpf_map_push_elem(&xdp_stats_map, &icmpinfo, BPF_EXIST);

done:
	return XDP_PASS;
}