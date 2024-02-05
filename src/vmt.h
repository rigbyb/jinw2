#pragma once

namespace vmt {
	void** get_vtable(void* ptr);
	void* get_method(void* ptr, unsigned int index);
	void override_vtable(void* ptr, void** table);

	class vmthook {
	public:
		vmthook(void* ptr);
		// Should not copy this
		vmthook(const vmthook& rval) = delete;
		vmthook& operator=(const vmthook& rval) = delete;
		vmthook(vmthook&& rval) noexcept;
		vmthook& operator=(vmthook&& rval) noexcept;
		~vmthook();

		void* get_original(unsigned int index);
		void set_method(unsigned int index, void* method);

	private:
		void* ptr_;
		void** original_table_;
		void** new_table_;
		unsigned int num_methods_;
	};
}