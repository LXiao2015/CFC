void updateTraffic(int path[], int update_path[], int demand) {
	for(int step = 1; path[step] > 0; ++step) {
		BW[path[step]-1][path[step-1]-1] += demand;
		BW[path[step-1]-1][path[step]-1] += demand;
	}
	for(int step = 1; update_path[step] > 0; ++step) {
		BW[update_path[step]-1][update_path[step-1]-1] -= demand;
		BW[update_path[step-1]-1][update_path[step]-1] -= demand;
	}
}
