// BCC_HASH() is a BCC macro that defines a hash table map
BPF_HASH(counter_table);

int hello(void *ctx) {
  u64 uid;
  u64 counter = 0;
  u64 *p;

  // bpf_get_current_uid_gid() is a helper function used to obtain the user ID
  // that is running the process that triggered this kprobe event. The user ID
  // is held in the lowest 32 bits of the 64-bit value that gets returned. (The
  // top 32 bits hold the group ID, but that part is masked out)
  uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;

  // Look for an entry in the hash table with a key matching the user ID. It
  // returns a pointer to the corresponding value in the hash table.
  p = counter_table.lookup(&uid);

  // If there is an entry for this user ID, set the counter variable to the
  // current value in the hash table (pointed to by p). If there is no entry for
  // this user ID in the hash table, the pointer will be 0, and the counter
  // value will be left at 0.
  if (p != 0) {
    counter = *p;
  }

  // Whatever the current counter value is, it gets incremented by one.
  counter++;

  // Update the hash table with the new counter value for this user ID.
  // Not proper C code, it's the BCC syntax instead!
  counter_table.update(&uid, &counter);
  return 0;
}
