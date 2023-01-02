#include "core/UUID.hpp"

namespace nf
{
	std::random_device UUID::s_seeder;
	std::mt19937_64 UUID::s_engine(s_seeder());
	std::uniform_int_distribution<std::uint64_t> UUID::s_dist;

	UUID::UUID()
		: m_uuid(s_dist(s_engine))
	{
	}

	UUID::UUID(std::uint64_t uuid)
		: m_uuid(uuid)
	{
	}
}

