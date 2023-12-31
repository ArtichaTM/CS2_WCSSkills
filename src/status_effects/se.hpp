#ifndef WCSSKILLS_SE_H
#define WCSSKILLS_SE_H

namespace stateff {
	class StatusEffect;
}

#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"
#include "../managers/infomanager.hpp"
#include "../events/event.hpp"

namespace stateff {

	enum SE_target {
		Self, Enemy, Teammate,	// Solo
		Team, EnemyTeam,		// Teams
		TeamExceptSelf,			// Except self
	};
	
	class StatusEffect {
	private:
		events::Function* eventReceiver = nullptr;

		void expire();
	public:
		StatusEffect(
			WCSPlayer* _owner,
			const managers::SEInfo* _info,
			const dataStorage::DataStorage* arguments,
			const float* multiplier
		);
		~StatusEffect();
		
		WCSPlayer* owner;
		const managers::SEInfo* info;
		dataStorage::DataStorage* arguments;
		
		events::ReturnEvent execute(std::shared_ptr<events::Event>);
		
		[[nodiscard]] traits::tr_set getTraits() const noexcept;
		[[nodiscard]] bool is_unremovable() const noexcept;
	};
} // namespace stateff

#endif //WCSSKILLS_SE_H
