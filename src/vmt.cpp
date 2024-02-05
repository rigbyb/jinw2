#include "vmt.h"
#include <algorithm>
#include <stdexcept>
#include <string>

void** vmt::get_vtable(void* ptr) {
    return *reinterpret_cast<void***>(ptr);
}

void* vmt::get_method(void* ptr, unsigned int index) {
    return get_vtable(ptr)[index];
}

void vmt::override_vtable(void* ptr, void** table) {
    *reinterpret_cast<void***>(ptr) = table;
}

vmt::vmthook::vmthook(void* ptr) {
    ptr_ = ptr;
    original_table_ = vmt::get_vtable(ptr_);

    num_methods_ = 0;

    while (original_table_[num_methods_])
        num_methods_++;

    new_table_ = new void* [num_methods_];

    std::copy(original_table_, original_table_ + num_methods_, new_table_);

    vmt::override_vtable(ptr_, new_table_);
}

vmt::vmthook::vmthook(vmthook&& rval) noexcept
    : ptr_(std::exchange(rval.ptr_, nullptr)),
    original_table_(std::exchange(rval.original_table_, nullptr)),
    new_table_(std::exchange(rval.new_table_, nullptr)),
    num_methods_(std::exchange(rval.num_methods_, 0)) {
}

vmt::vmthook& vmt::vmthook::operator=(vmthook&& rval) noexcept {
    if (&rval != this) {
        ptr_ = std::exchange(rval.ptr_, nullptr);
        original_table_ = std::exchange(rval.original_table_, nullptr);
        new_table_ = std::exchange(rval.new_table_, nullptr);
        num_methods_ = std::exchange(rval.num_methods_, 0);
    }

    return *this;
}

vmt::vmthook::~vmthook() {
    if (ptr_ && original_table_)
        vmt::override_vtable(ptr_, original_table_);

    if (new_table_)
        delete[] new_table_;
}

void* vmt::vmthook::get_original(unsigned int index) {
    if (index > num_methods_)
        throw std::runtime_error("Index " + std::to_string(index) + " greater than method count");

    return original_table_[index];
}

void vmt::vmthook::set_method(unsigned int index, void* method) {
    if (index > num_methods_)
        throw std::runtime_error("Index " + std::to_string(index) + " greater than method count");
    
    new_table_[index] = method;
}