#pragma once

#include <vector>
#include <functional>
#include <deque>
#include <atomic>

namespace reactive::v4 {

	namespace details {
	namespace Delegate {
		// 0 - reserved for empty tag?
		static std::atomic<unsigned long long> delegate_uuid{ 1 };	// should be enough approx. for 1000 years at 3Ghz continuous incrementation
	}
	}

	// pass by copy
	class DelegateTag {
		unsigned long long tag;

	protected:
		DelegateTag(std::nullptr_t)
			:tag(0)
		{}
		DelegateTag()
			: tag(details::Delegate::delegate_uuid.fetch_add(1))
		{}

	public:
		static DelegateTag make(){
			return {};
		}
		static DelegateTag empty(){
			return {nullptr};
		}

		bool operator==(const DelegateTag& other) const {
			return tag == other.tag;
		}
		bool operator!=(const DelegateTag& other) const {
			return tag != other.tag;
		}
	};

}
