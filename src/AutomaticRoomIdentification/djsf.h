#pragma once
#include <vector>
namespace DJSF {
	struct Set{
		std::vector<int> nodes;
		std::vector<int> parents;
		Set(int n) {		
			nodes = std::vector<int>(n);
			parents = std::vector<int>(n);
			for (int i = 0; i < n; i++) {
				parents[i] = i;
			}
		}
		int getRoot(int id) {
			int r = id;
			while (parents[r] != r) {
				r = parents[r];
			}
			int j = id;
			//collapse paths
			while (parents[j] != j) {
				int t = parents[j];
				parents[t] = r;
				j = t;
			}
			return r;
		}
		void merge(int i, int j) {
			int ri = getRoot(i);
			int rj = getRoot(j);
			parents[ri] = rj;
		}
	};
}