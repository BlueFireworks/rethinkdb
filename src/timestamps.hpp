#ifndef TIMESTAMPS_HPP_
#define TIMESTAMPS_HPP_

#include <stdint.h>

#include "rpc/serialize_macros.hpp"

/* These are the timestamp types used by the clustering code.
`repli_timestamp_t`, which is used internally within the btree code, is defined
elsewhere. */

/* `state_timestamp_t` is a unique identifier of a particular point in a
timeline. `transition_timestamp_t` is the unique identifier of a transition from
one `state_timestamp_t` to the next. Databases have `state_timestamp_t`s, and
transactions have `transition_timestamp_t`s. */

class state_timestamp_t {
public:
    bool operator==(state_timestamp_t t) const { return num == t.num; }
    bool operator!=(state_timestamp_t t) const { return num != t.num; }
    bool operator<(state_timestamp_t t) const { return num < t.num; }
    bool operator>(state_timestamp_t t) const { return num > t.num; }
    bool operator<=(state_timestamp_t t) const { return num <= t.num; }
    bool operator>=(state_timestamp_t t) const { return num >= t.num; }

    static state_timestamp_t zero() {
        state_timestamp_t t;
        t.num = 0;
        return t;
    }

    // TODO get rid of this. This is only for a hack until we know what to do with timestamps
    repli_timestamp_t to_repli_timestamp() const {
        repli_timestamp_t ts;
        ts.time = static_cast<uint32_t>(num);   // FIXME Aaaargh!!! We're losing precision here
        return ts;
    }

private:
    friend class transition_timestamp_t;
    friend std::ostream &operator<<(std::ostream &os, state_timestamp_t ts);
    uint64_t num;
    RDB_MAKE_ME_SERIALIZABLE_1(num);
};

class transition_timestamp_t {
public:
    bool operator==(transition_timestamp_t t) const { return before == t.before; }
    bool operator!=(transition_timestamp_t t) const { return before != t.before; }
    bool operator<(transition_timestamp_t t) const { return before < t.before; }
    bool operator>(transition_timestamp_t t) const { return before > t.before; }
    bool operator<=(transition_timestamp_t t) const { return before <= t.before; }
    bool operator>=(transition_timestamp_t t) const { return before >= t.before; }

    static transition_timestamp_t starting_from(state_timestamp_t before) {
        transition_timestamp_t t;
        t.before = before;
        return t;
    }

    state_timestamp_t timestamp_before() {
        return before;
    }

    state_timestamp_t timestamp_after() {
        state_timestamp_t after;
        after.num = before.num + 1;
        guarantee(after > before, "timestamp counter overflowed");
        return after;
    }

    // TODO get rid of this. This is only for a hack until we know what to do with timestamps
    uint64_t numeric_representation() {
        return before.num;
    }

    // TODO get rid of this. This is only for a hack until we know what to do with timestamps
    repli_timestamp_t to_repli_timestamp() const {
        return before.to_repli_timestamp();
    }

private:
    state_timestamp_t before;
    RDB_MAKE_ME_SERIALIZABLE_1(before);
};

#endif /* TIMESTAMPS_HPP_ */