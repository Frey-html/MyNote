To solve hash conflicts, which occur when two different keys generate the same index in a hash table, several methods can be employed. Here are some commonly used techniques:

1. **Chaining (Chain Addressing):**
   - In this method, each slot or bucket in the hash table contains a linked list or some other data structure that can hold multiple items.
   - When a collision occurs, the item is simply added to the linked list at the appropriate slot.
   - Advantages include simplicity and effectiveness when dealing with a high load factor.

2. **Open Addressing (Closed Hashing):**
   - This technique involves resolving collisions by finding another empty slot within the hash table for the element that caused the collision.
   - There are different strategies for finding an alternative slot:
     - **Linear Probing:** Where the next available slot is found by sequentially searching from the original index until an empty slot is found.
     - **Quadratic Probing:** Similar to linear probing but uses a quadratic function to determine the interval between probes.
     - **Double Hashing:** Uses a second hash function to compute the interval between probes.

3. **Rehashing (Secondary Hashing):**
   - If a collision occurs with the primary hash function, a secondary hash function is applied to the key to determine another index.
   - This process can be repeated with additional hash functions if necessary until a non-colliding slot is found.

4. **Cuckoo Hashing:**
   - This method allows for constant-time lookups and insertions under ideal conditions.
   - It uses two hash functions and two tables where elements are placed in one of the tables based on the output of the two functions.
   - When a new element arrives, it may evict an existing element to make space, and the displaced element is rehashed into the other table.

5. **Robin Hood Hashing:**
   - A variant of linear probing where the distance moved by a new entry depends on the difference in the distances already traveled by the existing entries.
   - It aims to minimize the average search time by reducing the clustering effect.

Each method has its own advantages and disadvantages, such as complexity, performance under varying load factors, and the amount of additional memory required. The choice of method often depends on the specific application requirements and the expected distribution of keys.