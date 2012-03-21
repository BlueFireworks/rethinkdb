#ifndef MEMCACHED_CLUSTERED_STORE_HPP_
#define MEMCACHED_CLUSTERED_STORE_HPP_

#include "memcached/store.hpp"
#include "memcached/protocol.hpp"

struct cluster_get_store_t : public get_store_t {
    explicit cluster_get_store_t(namespace_interface_t<memcached_protocol_t>& cluster_ns_);

    get_result_t get(const store_key_t& key, order_token_t token);

    rget_result_t rget(rget_bound_mode_t left_mode, const store_key_t &left_key, rget_bound_mode_t right_mode, const store_key_t &right_key, order_token_t token);

    ~cluster_get_store_t();
private:
    namespace_interface_t<memcached_protocol_t>& namespace_if;
};

struct cluster_set_store_t : public set_store_t {
    explicit cluster_set_store_t(namespace_interface_t<memcached_protocol_t>& cluster_ns_);

    mutation_result_t change(const mutation_t& m, castime_t cas, order_token_t token);

    ~cluster_set_store_t();
private:
    namespace_interface_t<memcached_protocol_t>& namespace_if;
};

#endif