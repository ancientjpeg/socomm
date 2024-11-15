# socomm - Dynamic Library Communication

`socomm` (for `.so` Communication) is a C++ library implemented on top of [libzmq](https://github.com/zeromq/libzmq) that implements (TODO: implement) coherent and predictable communication between code in multiple dynamic libraries that may be linked in the same or different processes. Its intention is to address the current headaches surrounding inter-plugin communication for common audio plugin wrapper formats (AU/VST/VST3/etc.), but is extensible to any software that struggles with a similar paradigm.

## Background

The current standard for software audio plugins involves implementing a shared library linked against multiple different plugin SDKs, generally including those for the industry standard AU, AAX, and VST/VST3 formats. The final piece of software output is a dynamic library which is linked by the host program, generally a DAW (Digital Audio Workstation) at runtime. Though useful for ease of development, this paradigm presents a number of challenges when multiple instances of the same plugin need to share data, which is becoming increasingly common as content libraries and novel inter-plugin coordination algorithms become the norm. Multiple hosts utilize different methods for linking in plugins - some sandbox them to different processes, while others link all child plugins into a single process that may or may not be the same process as the main host program. Furthermore, the multiple plugin formats are all viewed by the operating system as different code, meaning that `static` resources are not shared between multiple instances that are (aside from their wrapper format) functionally identical. This can lead to problems ranging from unexpected data synchronization behavior to [database corruption](https://www.sqlite.org/draft/howtocorrupt.html#multiple_copies_of_sqlite_linked_into_the_same_application). As such, a sane method for coordinating the action of multiple instances of dynamically linked libraries is necessary to ensure safe communication in audio plugin contexts.


## Goals
- Create a consistent communication layer for code that will be linked at runtime multiple times, possible through multiple different dynamic libraries
- Maintain a simple API to serialize/deserialize these shared data types, while also allowing users to use raw data and their serialization/deserialization method of choice
- *Possibly* extend the code to use memory mapping as a connection layer instead of sockets if its benefits performance
