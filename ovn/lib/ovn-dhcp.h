/*
 * Copyright (c) 2016 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OVN_DHCP_H
#define OVN_DHCP_H 1

#include "openvswitch/hmap.h"
#include "hash.h"

struct dhcp_opts_map {
    struct hmap_node hmap_node;
    char *name;
    char *type;
    size_t code;
};

#define DHCP_OPTION(NAME, CODE, TYPE) \
    {.name = NAME, .code = CODE, .type = TYPE}

#define OFFERIP              DHCP_OPTION("offerip", 0, "ipv4")
#define DHCP_OPT_NETMASK     DHCP_OPTION("netmask", 1, "ipv4")
#define DHCP_OPT_ROUTER      DHCP_OPTION("router", 3, "ipv4")
#define DHCP_OPT_DNS_SERVER  DHCP_OPTION("dns_server", 6, "ipv4")
#define DHCP_OPT_LOG_SERVER  DHCP_OPTION("log_server", 7, "ipv4")
#define DHCP_OPT_LPR_SERVER  DHCP_OPTION("lpr_server", 9, "ipv4")
#define DHCP_OPT_SWAP_SERVER DHCP_OPTION("swap_server", 16, "ipv4")

#define DHCP_OPT_POLICY_FILTER \
    DHCP_OPTION("policy_filter", 21, "ipv4")

#define DHCP_OPT_ROUTER_SOLICITATION \
    DHCP_OPTION("router_solicitation", 32, "ipv4")

#define DHCP_OPT_NIS_SERVER  DHCP_OPTION("nis_server", 41, "ipv4")
#define DHCP_OPT_NTP_SERVER  DHCP_OPTION("ntp_server", 42, "ipv4")
#define DHCP_OPT_SERVER_ID   DHCP_OPTION("server_id", 54, "ipv4")
#define DHCP_OPT_TFTP_SERVER DHCP_OPTION("tftp_server", 66, "ipv4")

#define DHCP_OPT_CLASSLESS_STATIC_ROUTE \
    DHCP_OPTION("classless_static_route", 121, "static_routes")
#define DHCP_OPT_MS_CLASSLESS_STATIC_ROUTE \
    DHCP_OPTION("ms_classless_static_route", 249, "static_routes")

#define DHCP_OPT_IP_FORWARD_ENABLE DHCP_OPTION("ip_forward_enable", 19, "bool")
#define DHCP_OPT_ROUTER_DISCOVERY DHCP_OPTION("router_discovery", 31, "bool")
#define DHCP_OPT_ETHERNET_ENCAP DHCP_OPTION("ethernet_encap", 36, "bool")

#define DHCP_OPT_DEFAULT_TTL DHCP_OPTION("default_ttl", 23, "uint8")

#define DHCP_OPT_TCP_TTL  DHCP_OPTION("tcp_ttl", 37, "uint8")
#define DHCP_OPT_MTU      DHCP_OPTION("mtu", 26, "uint16")
#define DHCP_OPT_LEASE_TIME DHCP_OPTION("lease_time", 51, "uint32")
#define DHCP_OPT_T1 DHCP_OPTION("T1", 58, "uint32")
#define DHCP_OPT_T2 DHCP_OPTION("T2", 59, "uint32")

static inline uint32_t
dhcp_opt_hash(char *opt_name)
{
    return hash_string(opt_name, 0);
}

static inline struct dhcp_opts_map *
dhcp_opts_find(const struct hmap *dhcp_opts, char *opt_name)
{
    struct dhcp_opts_map *dhcp_opt;
    HMAP_FOR_EACH_WITH_HASH (dhcp_opt, hmap_node, dhcp_opt_hash(opt_name),
                             dhcp_opts) {
        if (!strcmp(dhcp_opt->name, opt_name)) {
            return dhcp_opt;
        }
    }

    return NULL;
}

static inline void
dhcp_opt_add(struct hmap *dhcp_opts, char *opt_name, size_t code, char *type)
{
    struct dhcp_opts_map *dhcp_opt = xzalloc(sizeof *dhcp_opt);
    dhcp_opt->name = xstrdup(opt_name);
    dhcp_opt->code = code;
    dhcp_opt->type = xstrdup(type);
    hmap_insert(dhcp_opts, &dhcp_opt->hmap_node, dhcp_opt_hash(opt_name));
}

static inline void
dhcp_opts_destroy(struct hmap *dhcp_opts)
{
    struct dhcp_opts_map *dhcp_opt;
    HMAP_FOR_EACH_POP(dhcp_opt, hmap_node, dhcp_opts) {
        free(dhcp_opt->name);
        free(dhcp_opt->type);
        free(dhcp_opt);
    }
    hmap_destroy(dhcp_opts);
}

#endif /* OVN_DHCP_H */
