#include "network.h"
#include "random.h"
#include <iostream>
using namespace std;

void Network::resize(const size_t& n_values) {
	
	values.clear();
	vector<double> NewValues(n_values, 0.0);
	RNG.normal(NewValues);
	values = NewValues;
}

bool Network::add_link(const size_t& a, const size_t& b) {
	
	if(a != b and a >= 0 and b >= 0 and a < size() and b < size()) {	
		std::multimap<size_t,size_t>::iterator it;
		for(it=links.begin(); it!=links.end(); ++it) {
			if((it->first == a and it->second == b) or (it->first == b and it->second == a)) {
				return false;
			}
		}
		links.insert(std::pair<size_t,size_t>(a,b));
		links.insert(std::pair<size_t,size_t>(b,a));
		return true;
	}
	return false;
}

size_t Network::random_connect(const double& n_degree) {
	
	size_t links_created(0);
	links.clear();
	for(size_t i(0); i < size(); ++i) {
		int n_link(RNG.poisson(n_degree));
		vector<int> nodes_index(n_link, 0);
		RNG.uniform_int(nodes_index, 0, size()-1);
		for(int j(0); j < n_link; ++j) {
			if(add_link(i,nodes_index[j]) == true) {
				++links_created;
			}
		}
	}
	return links_created;
}

size_t Network::set_values(const std::vector<double>& vals) {
	
	size_t newVals_size(vals.size());
	size_t Size(size());
	if (newVals_size < Size) {
		Size = newVals_size;
	}
	for (size_t i(0); i < Size; ++i) {
		values[i] = vals[i];
	}
	return Size;
}

size_t Network::size() const {
	
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	
	return links.count(_n);
}

double Network::value(const size_t &_n) const {
	
	return values[_n];
}

vector<double> Network::sorted_values() const {
	
	vector<double> sorted_tab(values);
	for(size_t i(0); i < size(); ++i) {
		for(size_t j(i+1); j < size(); ++j) {
			if (sorted_tab[i] < sorted_tab[j]) {
				double sort(sorted_tab[i]);
				sorted_tab[i] = sorted_tab[j];
				sorted_tab[j] = sort;
			}
		}
	}
	return sorted_tab;
}

vector<size_t> Network::neighbors(const size_t& key_n) const {
	
	vector<size_t> key_neighbors;
	multimap<size_t, size_t>::const_iterator it = links.find(key_n);
	for(; it->first == key_n; ++it) {
		key_neighbors.push_back(it->second);
	}
	return key_neighbors;
}
