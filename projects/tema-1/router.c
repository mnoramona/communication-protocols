#include "queue.h"
#include "lib.h"
#include "protocols.h"

struct route_table_entry *rtable;
int rtable_len;

struct mac_entry *arp_table;
int arp_table_len;

struct route_table_entry *get_best_route(uint32_t ip_dest) {
	
	struct route_table_entry *candidate = NULL;

	for (int i = 0; i < rtable_len; i++)
	{
		if ((rtable[i].prefix & rtable[i].mask) == ip_dest && rtable[i].mask) {
			if (candidate == NULL || ntohl(rtable[i].mask) > ntohl(candidate->mask)) {
				candidate = &(rtable[i]);
			}
		}
	}

	return candidate;
}

struct mac_entry *get_mac_entry(uint32_t given_ip) {

	struct mac_entry *candidate = NULL;
	
	for (int i = 0; i < arp_table_len; i++)
	{
		if (arp_table[i].ip == given_ip)
			candidate = &(arp_table[i]);
	}

	return candidate;
}

int main(int argc, char *argv[])
{
	char buf[MAX_PACKET_LEN];

	// Do not modify this line
	init(argc - 2, argv + 2);


	while (1) {

		int interface;
		size_t len;

		interface = recv_from_any_link(buf, &len);
		DIE(interface < 0, "recv_from_any_links");

		struct ether_header *eth_hdr = (struct ether_header *) buf;
		/* Note that packets received are in network order,
		any header field which has more than 1 byte will need to be conerted to
		host order. For example, ntohs(eth_hdr->ether_type). The oposite is needed when
		sending a packet on the link, */
		/* DIE is a macro for sanity checks */

		DIE(rtable == NULL, "memory");

		

	}
}

