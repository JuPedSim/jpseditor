#pragma once
#include "Eigen/Dense"
#include "djsf.h"
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using Eigen::VectorXd;
using Eigen::Vector2d;
using Eigen::Vector3d;
const Eigen::IOFormat CleanFmt(Eigen::FullPrecision, 0, ", ", "\n", "[", "]");
const Eigen::IOFormat wallIDFormat(Eigen::FullPrecision, 0, ",", "\n", "(", ")", "", "");
const Eigen::IOFormat vertFormat(Eigen::FullPrecision, 0, ",", "_", "(", ")", "", "");
const Eigen::IOFormat debugFormat(7, 0, ",", "\n", "(", ")", "", "");

//for sorting with index return
typedef std::pair<int, double> indexValuePair;
bool compareIndexValuePair(const indexValuePair& l, const indexValuePair& r)
{
	return l.second < r.second;
}
typedef std::pair<std::string, double> stringDoublePair;
bool compareStringDoublePair(const stringDoublePair& l, const stringDoublePair& r)
{
	return l.second < r.second;
}
double Sqrt(double d) {
	return sqrt(d);
}
MatrixXd dist(MatrixXd samples) {
	MatrixXd distMat = MatrixXd(samples.rows(), samples.rows());
	for (int i = 0; i < samples.rows(); i++) {
		for (int j = i+1; j < samples.rows(); j++) {
			double d = (samples.row(i) - samples.row(j)).norm();
			distMat(i, j) = d;
			distMat(j, i) = d;
		}
	}
	return distMat;
}
bool lineIntersect(const Vector2d &a1, const Vector2d &a2, const Vector2d &b1, const Vector2d &b2) {
	Matrix2d A = Matrix2d();
	A.col(0) = a2 - a1;
	A.col(1) = b1-b2;
	double detA = A(0, 0)*A(1, 1) - A(0, 1)*A(1, 0);
	bool intersecting = false;
	if (detA != 0) {
		Matrix2d Ainv = Matrix2d();
		Ainv.row(0) = Vector2d( A(1, 1), -A(0, 1) ); 
		Ainv.row(1) = Vector2d(-A(1, 0), A(0, 0));
		Ainv = Ainv *(1.0 / detA);
		Vector2d solution = Ainv * (b1 - a1);
		if (solution[0] >= -0.0001 && solution[0] <= 1.0001 && solution[1] >= -0.0001 && solution[1] <= 1.0001) {
			intersecting = true;
		}
	}
	return intersecting;
}
std::vector<double> haltonSeq(int n, int b) {
	std::vector<double> vals = std::vector<double>(n);
	for (int i = 0; i < n; i++) {
		double j = (double)(i+1);
		double f = 1, r = 0;
		while (j > 0) {
			f = f / (double)b;
			r = r + f*((int)j % b);
			j = floor(j / (double)b);
		}
		vals[i] = r;
	}
	return vals;
}
std::vector<std::vector<double>> floorPlanFromFile(const char *filename) {
	int rows = 0;
	std::ifstream infile(filename);
	double a, b;
	std::list<std::vector<double>> data;
	while (infile >> a >> b) {
		data.push_back(std::vector<double>{a, b});
		rows++;
	}
	infile.close();
	std::vector<std::vector<double>> result(rows);
	int ix = 0;
	for (std::list<std::vector<double>>::iterator iter = data.begin(); iter != data.end(); iter++) {
		result[ix] = *iter;
		ix++;
	}
	return result;
	//return std::vector<std::vector<double>>();
}
int wallSide(const Vector2d &p, const Vector2d &w1, const Vector2d &w2) {
	int side = -1;
	Vector2d v = (w2 - w1);
	Vector2d pv = (p - w1);
	double test = pv[0] * v[1] + pv[1] * v[0];
	if (test > 0) {
		side = 1;
	}
	else {
		side = 0;
	}
	return(side);
}

VectorXd closestPoint (const Vector2d &p, const Vector2d &w1, const Vector2d &w2) {
	Vector2d v1 = (w2 - w1);
	double amt = ((p - w1).dot(v1.normalized()) / sqrt(v1.dot(v1)));
	return(w1 + std::min(1.0, std::max(0.0, amt))*v1);
}
std::string vertID(const Vector2d &v) {
	std::ostringstream strs;
	strs << v.format(wallIDFormat);
	return strs.str();
}
std::string wallID(const Vector2d &w1, const Vector2d &w2) {	
	std::ostringstream strs1,strs2;
	strs1 << w1.format(wallIDFormat);
	strs2 << w2.format(wallIDFormat);
	std::string sw1 = strs1.str();
	std::string sw2 = strs2.str();
	std::string id = sw1 + sw2;
	if (sw1 > sw2) id = sw2 + sw1; 
	return id;
}
std::string wallIDReverse(const Vector2d &w1, const Vector2d &w2) {
	std::ostringstream strs1, strs2;
	strs1 << w1.format(wallIDFormat);
	strs2 << w2.format(wallIDFormat);
	std::string sw1 = strs1.str();
	std::string sw2 = strs2.str();
	std::string id = sw2 + sw1;
	if (sw1 > sw2) id = sw1 + sw2;
	return id;
}
std::vector<int> connectedComponents(MatrixXd adjacency) {
//	std::cout << adjacency.format(CleanFmt) << std::endl;
	DJSF::Set forest = DJSF::Set(adjacency.rows());
	for (int i = 0; i < adjacency.rows(); i++) {
		for (int j = i + 1; j < adjacency.rows(); j++) {
			if (adjacency(i, j) == 1) {
				forest.merge(i, j);
			}
		}
	}
	std::vector<int> result = std::vector<int>(adjacency.rows());
	for (int i = 0; i < adjacency.rows(); i++){
		result[i] = forest.getRoot(i);
	}
	return result;
}
std::map<int,int> unique(std::vector<int> values) {
	std::map<int, int> uvalmap = std::map<int, int>();
	for (int i = 0; i < values.size(); i++) {
		uvalmap[values[i]] = 1;
	}	
	return uvalmap;
}
std::vector<std::list<double>> getRooms(std::vector<std::vector<double>> floorPlan) {
	
	
	//values are x1,y1,x2,y2
	std::map<std::string, std::vector<double>> edges;
	std::map<std::string, std::list<std::string>> vertexEdges;
	std::map<std::string, std::vector<double>> vertices;
	//this will be the length of the longest wall
	double maxCheckDist = 0;
	for (int i = 0; i < floorPlan.size() / 2; i++) {
		Vector2d w1 = Vector2d{ floorPlan[2 * i][0],floorPlan[2 * i][1] }, w2 = Vector2d{ floorPlan[2 * i + 1][0],floorPlan[2 * i + 1][1] };
		double wallLength = (w1 - w2).norm();
		maxCheckDist = maxCheckDist < wallLength ? wallLength : maxCheckDist;
		std::string wallid = wallID(w1,w2);
		edges[wallid] = std::vector<double>{ floorPlan[2*i][0],floorPlan[2*i][1],floorPlan[2*i + 1][0],floorPlan[2*i + 1][1] };
		if (vertexEdges.count(vertID(w1))==0) {
			vertexEdges[vertID(w1)] = std::list<std::string>();			
		}
		if (vertexEdges.count(vertID(w2))==0) {
			vertexEdges[vertID(w2)] = std::list<std::string>();
		}
		vertices[vertID(w1)] = {w1[0],w1[1]};
		vertices[vertID(w2)] = {w2[0],w2[1]};
		if (std::find(vertexEdges[vertID(w1)].begin(), vertexEdges[vertID(w1)].end(), wallid) == vertexEdges[vertID(w1)].end()) {
			vertexEdges[vertID(w1)].push_back(wallid);
		}
		if (std::find(vertexEdges[vertID(w2)].begin(), vertexEdges[vertID(w2)].end(), wallid) == vertexEdges[vertID(w2)].end()) {
			vertexEdges[vertID(w2)].push_back(wallid);
		}
		
	}
	double epsilon = 0.01;
	maxCheckDist = maxCheckDist + 4 * epsilon;
	MatrixXd prm = MatrixXd(edges.size() * 2, 2);
	std::vector<std::vector<std::string>> prmEdgeMap = std::vector<std::vector<std::string>>(prm.rows());
	int pix = 0;
	// sort the edges in radial order, build prm as we go
	for (std::map<std::string, std::vector<double>>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		std::string vert = iter->first;
		Vector2d v = Vector2d(iter->second[0], iter->second[1]);
		std::list<std::string> wallids = vertexEdges[vert];
		std::vector<stringDoublePair> angles = std::vector<stringDoublePair>(wallids.size());
		std::vector<double> firstWall = edges[wallids.front()];
		//needs to be vector going out FROM current vertex
		double firstWallx, firstWally;
		if (vertID(Vector2d(firstWall[0], firstWall[1])) == vert) {
			firstWallx = firstWall[2] - firstWall[0];
			firstWally = firstWall[3] - firstWall[1];
		}
		else {
			firstWallx = firstWall[0] - firstWall[2];
			firstWally = firstWall[1] - firstWall[3];
		};
		
		int ix = 0;
		for (std::list<std::string>::iterator walliter = wallids.begin(); walliter != wallids.end(); ++walliter) {
			std::vector<double> wall = edges[*walliter];
			double wallx, wally;
			if (vertID(Vector2d(wall[0], wall[1])) == vert) {
				wallx = wall[2] - wall[0];
				wally = wall[3] - wall[1];
			}
			else {
				wallx = wall[0] - wall[2];
				wally = wall[1] - wall[3];
			};
			Vector3d v1 = Vector3d(firstWallx, firstWally, 0.0).normalized();
			Vector3d v2 = Vector3d(wallx, wally, 0.0).normalized();
			Vector3d v1xv2 = v1.cross(v2);
			double signs[2] = { 1,-1 };
			double angle = asin(v1xv2.norm());
			//sign of dot product tells us whether we are > 90
			if (v1.dot(v2) < 0) {
				//if we are greater than 90, must calculate the complement of the angle from 180, as asin only returns between 0 and 90
				// (asin returns between -90 and 90 but we are handing it a norm, which is always positive, so we can be confident a subtraction is correct)
				angle = EIGEN_PI - angle;
			}
			//sign of cross product z component tells us which side of 180 we are on.
			//if the z component is negative, we have < 180 degrees and want to "add" it to 360 which we will get by subtracting a negative.
			//if the z component is positive, we have > 180 degrees and want to subtract it from 360 (so leave the angle positive, since we are subtracting it in our final computation)
                        angle = angle * signs[(int)std::signbit(v1xv2[2])];
			//this is clockwise angle, so subtract it from 360 and mod 360
			double angleRad = fmod(2.0 * EIGEN_PI - angle, 2.0*EIGEN_PI);
			if (angleRad == 0) {
				//need to check if this is 180 case or 0 case
				if (v1.dot(v2) < 0) {
					angleRad = EIGEN_PI;
				}
			}
			stringDoublePair pair = stringDoublePair{ *walliter,angleRad };
			angles[ix] = pair;
			ix++;
		}
		//std::cout << " sorting edges for vertex " << v.format(vertFormat) << std::endl;
		std::sort(angles.begin(), angles.end(), &compareStringDoublePair);
		vertexEdges[vert] = std::list<std::string>();
		//now that edges are sorted for this vertex, place prm points at all half angles
		for (int i = 0; i < angles.size(); i++) {
			//might as well update the edge list for the vertex to be in order
			vertexEdges[vert].push_back(angles[i].first);
			std::string otherVertex = vertID(Vector2d(edges[angles[i].first][0], edges[angles[i].first][1]));
			if (otherVertex.compare(vert) == 0) {
				otherVertex = vertID(Vector2d(edges[angles[i].first][2], edges[angles[i].first][3]));
			}
			std::string edge1 = wallID(v, Vector2d(vertices[otherVertex][0],vertices[otherVertex][1]));
			Vector2d wall1 = v - Vector2d(vertices[otherVertex][0],vertices[otherVertex][1]);
			//take the leftward perpendicular (will always work for bisecting unless the two vectors are equal, which should never happen)
			wall1 = Vector2d(-wall1[1], wall1[0]).normalized();
			int nextWall = (i + 1) % angles.size();
			otherVertex = vertID(Vector2d(edges[angles[nextWall].first][0], edges[angles[nextWall].first][1]));
			if (otherVertex.compare(vert) == 0) {
				otherVertex = vertID(Vector2d(edges[angles[nextWall].first][2], edges[angles[nextWall].first][3]));
			}
			std::string edge2 = wallID(v, Vector2d(vertices[otherVertex][0], vertices[otherVertex][1]));
			Vector2d wall2 = Vector2d(vertices[otherVertex][0], vertices[otherVertex][1])-v;
			wall2 = Vector2d(-wall2[1], wall2[0]).normalized();			
			Vector2d bisector = (wall1 + wall2).normalized() * epsilon;			
			prm.row(pix) = v+bisector;
			prmEdgeMap[pix] = std::vector<std::string>{ edge1, edge2 };
			pix++;
		}		
	}
	MatrixXd walls = MatrixXd(edges.size(), 4);
	int ix = 0;
	for (std::map<std::string, std::vector<double>>::iterator iter = edges.begin(); iter != edges.end(); ++iter)
	{
		std::string k = iter->first;
		std::vector<double> w = iter->second;
		walls.row(ix) << w[0], w[1], w[2], w[3];
		ix++;
	}
	MatrixXd prmDist = dist(prm);
//	std::cout << "PRM Distances: " << std::endl << prmDist.format(wallIDFormat) << std::endl;
	MatrixXd prmSortedNeighbors = MatrixXd(prm.rows(), prm.rows());
	//sort the distances
	for (int i = 0; i < prm.rows(); i++) {
		std::vector<indexValuePair> v = std::vector<indexValuePair>(prmDist.cols());
		for (int j = 0; j < prm.rows(); j++) {
			v[j] = indexValuePair{ j,prmDist(i,j)};
		}
		std::sort(v.begin(),v.end(), &compareIndexValuePair);
		for (int j = 0; j < prm.rows(); j++) {
			prmSortedNeighbors(i, j) = v[j].first;
		}
	}
//	std::cout << "Sorted Neighbors: "<<std::endl << prmSortedNeighbors.format(wallIDFormat) << std::endl;
	MatrixXd pointWallDist = MatrixXd(prm.rows(), edges.size());
	for (int i = 0; i < prm.rows(); i++) {
		for (int w = 0; w < walls.rows(); w++) {
			VectorXd wall = walls.row(w);
			VectorXd closest = closestPoint(prm.row(i), walls.block(w, 0, 1, 2).row(0), walls.block(w, 2, 1, 2).row(0));
			pointWallDist(i, w) = Vector2d(prm(i,0) - closest[0], prm(i,1)-closest[1]).norm();
		}
	}
	MatrixXd prmSortedWallDists = MatrixXd(prm.rows(), walls.rows());
	//sort the distances
	for (int i = 0; i < prm.rows(); i++) {
		std::vector<indexValuePair> v = std::vector<indexValuePair>(pointWallDist.cols());
		for (int j = 0; j < pointWallDist.cols(); j++) {
			v[j] = indexValuePair{ j,pointWallDist(i,j) };
		}
		std::sort(v.begin(), v.end(), &compareIndexValuePair);
		for (int j = 0; j < pointWallDist.cols(); j++) {
			prmSortedWallDists(i, j) = v[j].first;
		}
	}
	MatrixXd adjacency = MatrixXd::Constant(prm.rows(), prm.rows(),0.0);
    //std::cout << prm.format(debugFormat) << std::endl;
	for (int i = 0; i < prm.rows(); i++) {
		VectorXd closest = prmSortedNeighbors.block(i, 1, 1, prmSortedNeighbors.cols() - 1).row(0);
		Vector2d v1 = Vector2d(prm(i, 0), prm(i, 1));
		int connections = 0;
		int breaches = 0;
		//rows - 1 because we won't check actual closest point, which is the same point
		int j = 0;
		while (j < (prm.rows() - 1) &&  prmDist(i, closest[j]) < maxCheckDist) {
			bool connected = true;
			Vector2d v2 = Vector2d(prm(closest[j], 0), prm(closest[j], 1));
			for (int w = 0; w < walls.rows(); w++ ){
				if (pointWallDist(i, prmSortedWallDists(i, w)) > prmDist(i, closest[j])) { break; }
				VectorXd wall = walls.row(prmSortedWallDists(i, w));
				Vector2d w1 = Vector2d(wall[0], wall[1]);
				Vector2d w2 = Vector2d(wall[2],wall[3]);
				bool test = lineIntersect(v1, v2, w1, w2);
				if (test) {
					connected = false;
				}				
			}			
			if (connected && (connections < 10)) {
//				//std::cout << v1.format(vertFormat) << " is connected to " << v2.format(vertFormat) << std::endl;
				adjacency(i, closest[j]) = 1;
				adjacency(closest[j], i) = 1;
			}
			j++;
		}	
	}
    //std::cout << adjacency.format(debugFormat);
	std::vector<int> groups = connectedComponents(adjacency);
    //std::cout << "Groups: " << std::endl;
	for (int i = 0; i < groups.size(); i++) {
        //std::cout << groups[i] << std::endl;
	}
	std::map<int,int> groupToRoom = unique(groups);
	std::vector<int> roomToGroup = std::vector<int>(groupToRoom.size());
	ix = 0;
	for (std::map<int, int>::iterator iter = groupToRoom.begin(); iter != groupToRoom.end(); ++iter)
	{
		roomToGroup[ix] = iter->first;
		groupToRoom[iter->first] = ix;
		ix++;
	}	
	std::vector<std::map<std::string,std::vector<double>>> roomWalls = std::vector<std::map<std::string, std::vector<double>>>(roomToGroup.size());
	for (int i = 0; i < roomWalls.size(); i++) {
		roomWalls[i] = std::map<std::string, std::vector<double>>();
	}
	for (int i = 0; i < (prm.rows() - 1); i++) {
		int room = groupToRoom[groups[i]];
		//do for both edges that generated this point
		for (int w = 0; w < 2; w++) {
			std::string wID = prmEdgeMap[i][w];
			std::vector<double> wall = edges[wID];
			Vector2d w1 = Vector2d(wall[0], wall[1]);
			Vector2d w2 = Vector2d(wall[2], wall[3]);
			wID = wallID(w1, w2);
			std::string wIDR = wallIDReverse(w1, w2);
			if (wallSide(prm.row(i), w1, w2) == 0) {
				//flip for roomflipped
				roomWalls[room][wIDR] = std::vector<double>{ w2[0],w2[1],w1[0],w1[1] };
			}
			else {
				roomWalls[room][wID] = std::vector<double>{ w1[0],w1[1],w2[0],w2[1] };
			}
		}
	}	
	std::vector<std::list<double>> results = std::vector<std::list<double>>(roomToGroup.size());
	for (int i = 0; i < roomToGroup.size(); i++) {
		results[i] = std::list<double>();
		//std::cout << "Room " << i << ":" << std::endl;
		for (std::map<std::string, std::vector<double>>::iterator iter = roomWalls[i].begin(); iter != roomWalls[i].end(); ++iter)
		{
			std::string wID = iter->first;
			std::vector<double> points = iter->second;
			results[i].push_back(points[0]);
			results[i].push_back(points[1]);
			results[i].push_back(points[2]);
			results[i].push_back(points[3]);
            //std::cout << "(" << points[0] << ", " << points[1] << ") -- (" << points[2] << ", " << points[3] << ")" << std::endl;
		}
	}
	return results;
}

