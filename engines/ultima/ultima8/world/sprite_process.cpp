/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "ultima/ultima8/misc/pent_include.h"
#include "ultima/ultima8/world/sprite_process.h"
#include "ultima/ultima8/world/item_factory.h"
#include "ultima/ultima8/world/item.h"
#include "ultima/ultima8/world/current_map.h"
#include "ultima/ultima8/kernel/kernel.h"
#include "ultima/ultima8/world/get_object.h"

namespace Ultima {
namespace Ultima8 {

// p_dynamic_class stuff
DEFINE_RUNTIME_CLASSTYPE_CODE(SpriteProcess, Process)

SpriteProcess::SpriteProcess()
	: Process() {

}

SpriteProcess::SpriteProcess(int Shape, int Frame, int LastFrame,
                             int Repeats, int Delay, int x, int y, int z,
                             bool delayed_init) :
	_shape(Shape), _frame(Frame), _firstFrame(Frame), _lastFrame(LastFrame),
	_repeats(Repeats), _delay(Delay * 2), _x(x), _y(y), _z(z), _delayCounter(0),
	_initialized(false) {
	if (!delayed_init)
		init();
}

void SpriteProcess::init() {
	Item *item = ItemFactory::createItem(_shape, _frame, 0, Item::FLG_DISPOSABLE,
		0, 0, Item::EXT_SPRITE, true);
	item->move(_x, _y, _z);
	setItemNum(item->getObjId());
	_initialized = true;
}

SpriteProcess::~SpriteProcess(void) {
	Item *item = getItem(_itemNum);
	if (item) item->destroy();
}

void SpriteProcess::run() {
	if (!_initialized) init();

	Item *item = getItem(_itemNum);

	if (!item || (_frame > _lastFrame && _repeats == 1 && !_delayCounter)) {
		terminate();
		return;
	}

	if (_delayCounter) {
		_delayCounter = (_delayCounter + 1) % _delay;
		return;
	}

	if (_frame > _lastFrame) {
		_frame = _firstFrame;
		_repeats--;
	}

	item->setFrame(_frame);
	_frame++;
	_delayCounter = (_delayCounter + 1) % _delay;
}

// createSprite(_shape, _frame, end,               _delay, _x, _y, _z);
// createSprite(_shape, _frame, end, unk, _repeats, _delay, _x, _y, _z);
uint32 SpriteProcess::I_createSprite(const uint8 *args, unsigned int argsize) {
	int repeats = 1;
	ARG_SINT16(_shape);
	ARG_SINT16(_frame);
	ARG_SINT16(_lastFrame);

	if (argsize == 18) {
		ARG_SINT16(unknown);
		ARG_SINT16(repeats_count);
		repeats = repeats_count;
	}

	ARG_SINT16(_delay);
	ARG_UINT16(_x);
	ARG_UINT16(_y);
	ARG_UINT8(_z);
	Process *p = new SpriteProcess(_shape, _frame, _lastFrame, repeats, _delay, _x, _y, _z);
	return Kernel::get_instance()->addProcess(p);
}

void SpriteProcess::saveData(Common::WriteStream *ws) {
	Process::saveData(ws);

	ws->writeUint32LE(static_cast<uint32>(_shape));
	ws->writeUint32LE(static_cast<uint32>(_frame));
	ws->writeUint32LE(static_cast<uint32>(_firstFrame));
	ws->writeUint32LE(static_cast<uint32>(_lastFrame));
	ws->writeUint32LE(static_cast<uint32>(_repeats));
	ws->writeUint32LE(static_cast<uint32>(_delay));
	ws->writeUint32LE(static_cast<uint32>(_x));
	ws->writeUint32LE(static_cast<uint32>(_y));
	ws->writeUint32LE(static_cast<uint32>(_z));
	ws->writeUint32LE(static_cast<uint32>(_delayCounter));
	ws->writeByte(_initialized ? 1 : 0);
}

bool SpriteProcess::loadData(Common::ReadStream *rs, uint32 version) {
	if (!Process::loadData(rs, version)) return false;

	_shape = static_cast<int>(rs->readUint32LE());
	_frame = static_cast<int>(rs->readUint32LE());
	_firstFrame = static_cast<int>(rs->readUint32LE());
	_lastFrame = static_cast<int>(rs->readUint32LE());
	_repeats = static_cast<int>(rs->readUint32LE());
	_delay = static_cast<int>(rs->readUint32LE());
	_x = static_cast<int>(rs->readUint32LE());
	_y = static_cast<int>(rs->readUint32LE());
	_z = static_cast<int>(rs->readUint32LE());
	_delayCounter = static_cast<int>(rs->readUint32LE());
	_initialized = (rs->readByte() != 0);

	return true;
}

} // End of namespace Ultima8
} // End of namespace Ultima
