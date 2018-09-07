# STM-Linklist-sychrobench
The work uses IITH STM lib (in house STM library developed at IIT Hyderabad) and implements a set application (based on list) to test the performance of state of art concurrency protocols with the various concurrency mechanism in Synchrobench.

State of art concurrency control protocols used in IITH STM:

                  + 2 Phase locking
                  + Serialization Graph testing
                  + Multiversion concurency control

The concurrent Set (list based) application was evaluated against the concurrent list based data structures of Synchrobench:

                  + lazy-list
                  + lock-free list
                  + lock-coupling list
                  + Elastic Software Transactional Memory
                  
Note: code provided here is bare minimum. Interested researchers can contact for further details :).


Paper based on the work published at PARCOMPTECH 2017:

https://www.semanticscholar.org/paper/Performance-comparison-of-various-STM-concurrency-Singh-Peri/e6a23b12314aab52cdd2c8f04e69920f1026a8c1

