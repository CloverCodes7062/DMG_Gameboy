#pragma once
#include <vector>

class Bus
{
public:
	Bus();
	~Bus();

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

private:
	std::vector<uint8_t> ram;
};


