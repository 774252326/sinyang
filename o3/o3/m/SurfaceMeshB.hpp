#include "SurfaceMeshA.hpp"

class SurfaceMeshB : public SurfaceMesh
{
public:
	std::vector< std::vector<Point2D> > contourp;
	std::vector<double> contourValue;

	std::vector<double> triangleValue;
	SurfaceMeshA sfma;

	public:
	SurfaceMeshB(void){};
	~SurfaceMeshB(void){};

	void Clear()
	{
		SurfaceMesh::Clear();
		triangleValue.clear();		
	};

	void ClearContour()
	{
		contourp.clear();
		contourValue.clear();
	};


	void GetEdgePoint(std::vector<size_t> &edgep, std::vector<double> &edgepValue)
	{
		std::vector<size_t> edgep0;
		std::vector<size_t> edgep1;
		std::vector<size_t> ti;
		FindEdge(edgep0,edgep1,ti);

		edgep0.resize(edgep0.size()+edgep1.size());
		std::copy_backward(edgep1.begin(),edgep1.end(),edgep0.end());


		std::sort(edgep0.begin(),edgep0.end());
		std::vector<size_t>::iterator it;
		it=std::unique(edgep0.begin(),edgep0.end());

		edgep.assign(edgep0.begin(),it);
		edgepValue.assign(edgep.size(),0);

		for(size_t i=0;i<edgep.size();i++){
			std::vector<double> dist2(point2triangle[edgep[i]].size());
			for(size_t j=0;j<dist2.size();j++){
				dist2[j]=Point2D::Dist2(point[edgep[i]],sfma.point[point2triangle[edgep[i]][j]]);
			}
			size_t mi=std::min_element(dist2.begin(),dist2.end())-dist2.begin();
			edgepValue[i]=sfma.pointValue[point2triangle[edgep[i]][mi]];
		}


	};

	void Init()
	{
		sfma.point.assign(triangle.size(),Point2D());
		for(size_t i=0;i<sfma.point.size();i++){
			sfma.point[i]=point[triangle[i][0]]+point[triangle[i][1]]+point[triangle[i][2]];
			sfma.point[i].x[0]/=3;
			sfma.point[i].x[1]/=3;
		}
		sfma.pointValue.assign(triangleValue.begin(),triangleValue.end());


		std::vector<size_t> edgepoint;
		std::vector<double> edgepointv;

		GenPointToFaceMap();
		GetEdgePoint(edgepoint,edgepointv);

		for(size_t i=0;i<edgepoint.size();i++){
			sfma.point.push_back(point[edgepoint[i]]);
			sfma.pointValue.push_back(edgepointv[i]);
		}

		bool flag=sfma.Delaunay2DMesh();
		sfma.GenPointToFaceMap();
		sfma.ClearContour();
	};

	void GenContourMap(const std::vector<double> &vl)
	{
		sfma.GenContourMap(vl);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

	void GenContourMap(size_t Ncontour, double vmin, double vmax)
	{
		sfma.GenContourMap(Ncontour,vmin,vmax);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

	void GenContourMap(size_t Ncontour=10)
	{
		sfma.GenContourMap(Ncontour);
		contourp.resize(contourp.size()+sfma.contourp.size());
		std::copy_backward(sfma.contourp.begin(),sfma.contourp.end(),contourp.end());
		contourValue.resize(contourValue.size()+sfma.contourValue.size());
		std::copy_backward(sfma.contourValue.begin(),sfma.contourValue.end(),contourValue.end());
	};

};