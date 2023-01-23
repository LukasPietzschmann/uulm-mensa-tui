#pragma once

#include <cmath>

#include "CanShowDetailsOf.hpp"
#include "ListView.hpp"
#include "WindowLike.hpp"
#include "utils.hpp"

template <typename L, typename D, typename = InstanceOf<D, CanShowDetailsOf<L>>,
	// width, height, x, y, window: Arguments to WindowLike
	typename = ConstructableWith<D, unsigned, unsigned, unsigned, unsigned, WINDOW*>>
class MasterSlave : public WindowLike {
public:
	MasterSlave(unsigned width, unsigned height, unsigned x, unsigned y) : WindowLike(nullptr, width, height, x, y) {
		unsigned master_width = floor(width * 0.1);
		m_master = new ListView<L>(ListView<L>::VERTICAL, master_width, height, x, y);
		m_slave = new D(width - master_width, height, x + master_width, y);
	}

	~MasterSlave() {
		delete m_master;
		delete m_slave;
	}

	void prepare_refresh() override {
		m_master->prepare_refresh();
		if(const auto& element = m_master->get_selected_element(); element.has_value())
			m_slave->show(element.value());
		else
			m_slave->clear();
		m_slave->prepare_refresh();
	}

	void set_bg(int bg) override {
		m_master->set_bg(bg);
		m_slave->set_bg(bg);
	}

	template <typename... Args, typename = ConstructableWith<L, Args...>>
	void emplace_back(Args&&... args) {
		m_master->emplace_back(std::forward<Args>(args)...);
	}

	bool select_elem(unsigned index, SelectAction action = SelectAction::SELECT) {
		return m_master->select_elem(index, action);
	}

	void unselect_elem() { m_master->unselect_elem(); }

	unsigned size() const { return m_master->size(); }

private:
	ListView<L>* m_master;
	D* m_slave;
};
