#include <geometric_shapes/mesh_operations.h>
#include <geometric_shapes/shape_operations.h>
#include <ros/ros.h>
#include <shape_msgs/Mesh.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>

#include <boost/bind.hpp>

visualization_msgs::Marker loadMeshAsMarker(std::string const & resource, std::string const & frame) {
	shapes::Mesh * mesh = shapes::createMeshFromResource(resource);
	if (!mesh) {
		throw std::runtime_error("Failed to load mesh from resource: " + resource);
	}

	visualization_msgs::Marker marker;
	if (!shapes::constructMarkerFromShape(mesh, marker, true)) {
		throw std::runtime_error("Failed to construct marker from mesh: " + resource);
	}

	marker.action             = marker.ADD;
	marker.header.frame_id    = frame;
	marker.pose.orientation.w = 1;
	marker.color.a            = 1;

	return marker;
}


void republish(visualization_msgs::MarkerArray & markers, ros::Publisher & publisher) {
	for (std::size_t i = 0; i < markers.markers.size(); ++i) {
		markers.markers[i].header.stamp = ros::Time::now();
	}
	publisher.publish(markers);
}

int main(int argc, char * * argv) {
	ros::init(argc, argv, "test_load_mesh");
	ros::NodeHandle node("~");

	std::string frame = "map";
	node.getParam("frame", frame);

	visualization_msgs::MarkerArray markers;
	markers.markers.push_back(loadMeshAsMarker("package://test_load_mesh/beam_z_up.dae", frame));
	markers.markers.back().ns              = "beam_z_up.dae";
	markers.markers.back().color.r         = 1;
	markers.markers.back().pose.position.x = -1;
	markers.markers.push_back(loadMeshAsMarker("package://test_load_mesh/beam_y_up.dae", frame));
	markers.markers.back().ns              = "beam_y_up.dae";
	markers.markers.back().color.g         = 1;
	markers.markers.back().pose.position.x = 0;
	markers.markers.push_back(loadMeshAsMarker("package://test_load_mesh/beam.stl", frame));
	markers.markers.back().ns              = "beam.stl";
	markers.markers.back().color.b         = 1;
	markers.markers.back().pose.position.x = 1;

	ros::Publisher publisher = node.advertise<visualization_msgs::MarkerArray>("markers", 1, true);
	republish(markers, publisher);
	ROS_INFO_STREAM("Published and latched marker.");

	ros::Timer timer = node.createTimer(ros::Duration(1), boost::bind(&republish, markers, publisher));

	ros::spin();
}
