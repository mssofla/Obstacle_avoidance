#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
#include <moveit_msgs/msg/attached_collision_object.hpp>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit_visual_tools/moveit_visual_tools.h>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("move_group_demo");
int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions node_options;
  node_options.automatically_declare_parameters_from_overrides(true);
  auto move_group_node = rclcpp::Node::make_shared("move_group_interface_tutorial", node_options);

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(move_group_node);
  std::thread([&executor]() { executor.spin(); }).detach();

  // MoveIt operates on sets of joints called "planning groups" and stores them in an object called
  // the ``JointModelGroup``. Throughout MoveIt, the terms "planning group" and "joint model group"
  // are used interchangeably.
  static const std::string PLANNING_GROUP = "panda_arm";
  moveit::planning_interface::MoveGroupInterface move_group(move_group_node, PLANNING_GROUP);
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
  const moveit::core::JointModelGroup* joint_model_group =
  move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

  namespace rvt = rviz_visual_tools;
  moveit_visual_tools::MoveItVisualTools visual_tools(move_group_node, "panda_link0", "move_group_tutorial",
                                                      move_group.getRobotModel());
  visual_tools.deleteAllMarkers();
  visual_tools.loadRemoteControl();
  Eigen::Isometry3d text_pose = Eigen::Isometry3d::Identity();
  text_pose.translation().z() = 1.0;

  // We can print the name of the reference frame for this robot.
  RCLCPP_INFO(LOGGER, "Planning frame: %s", move_group.getPlanningFrame().c_str());

  // We can also print the name of the end-effector link for this group.
  RCLCPP_INFO(LOGGER, "End effector link: %s", move_group.getEndEffectorLink().c_str());

  // We can get a list of all the groups in the robot:
  RCLCPP_INFO(LOGGER, "Available Planning Groups:");
  std::copy(move_group.getJointModelGroupNames().begin(), move_group.getJointModelGroupNames().end(),
            std::ostream_iterator<std::string>(std::cout, ", "));

  //  we create an pointer that references the current robot's state.
  moveit::core::RobotStatePtr current_state = move_group.getCurrentState(10);
  std::vector<double> joint_group_positions;
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success;




  ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");

  geometry_msgs::msg::Pose target_pose;
  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.7;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);


  move_group.setMaxVelocityScalingFactor(0.15);
  move_group.setMaxAccelerationScalingFactor(0.15);

  // Compute the plan, visualize the trajectory, and execute it if tha plan was successful.
  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();

  if(success) 
   move_group.execute(my_plan);
  else 
   RCLCPP_INFO(LOGGER, "Planning failed!");

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
 


  ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");


  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.28;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);


  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();

  if(success) 
   move_group.execute(my_plan);
  else 
   RCLCPP_INFO(LOGGER, "Planning failed!");

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
 

 /////////////////////////////////////////////////////Add box/////////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");


  // Now, let's define a collision object ROS message for the robot to avoid.
  moveit_msgs::msg::CollisionObject collision_object;
  collision_object.header.frame_id = move_group.getPlanningFrame();

  // The id of the object is used to identify it.
  collision_object.id = "box1";

  // Define a box to add to the world.
  shape_msgs::msg::SolidPrimitive primitive;
  primitive.type = primitive.BOX;
  primitive.dimensions.resize(3);
  primitive.dimensions[primitive.BOX_X] = 0.1;
  primitive.dimensions[primitive.BOX_Y] = 1.5;
  primitive.dimensions[primitive.BOX_Z] = 0.5;

  // Define a pose for the box (specified relative to frame_id).
  geometry_msgs::msg::Pose box_pose;
  box_pose.orientation.w = 1.0;
  box_pose.position.x = 0.48;
  box_pose.position.y = 0.0;
  box_pose.position.z = 0.25;

  collision_object.primitives.push_back(primitive);
  collision_object.primitive_poses.push_back(box_pose);
  collision_object.operation = collision_object.ADD;

  std::vector<moveit_msgs::msg::CollisionObject> collision_objects;
  collision_objects.push_back(collision_object);

  // Now, let's add the collision object into the world
  // (using a vector that could contain additional objects)
  RCLCPP_INFO(LOGGER, "Add an object into the world");
  planning_scene_interface.addCollisionObjects(collision_objects);



  ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////

  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.7;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);


  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();

  if(success) 
   move_group.execute(my_plan);
  else 
   RCLCPP_INFO(LOGGER, "Planning failed!");

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
 


  ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");


  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.28;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);


  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();

  if(success) 
   move_group.execute(my_plan);
  else 
   RCLCPP_INFO(LOGGER, "Planning failed!");

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
 

  //////////////////////////////////////////////Add cylinder////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");



  // You can attach an object to the robot, so that it moves with the robot geometry.
  // This simulates picking up the object for the purpose of manipulating it.
  // The motion planning should avoid collisions between objects as well.
  moveit_msgs::msg::CollisionObject object_to_attach;
  object_to_attach.id = "cylinder1";

  shape_msgs::msg::SolidPrimitive cylinder_primitive;
  cylinder_primitive.type = primitive.CYLINDER;
  cylinder_primitive.dimensions.resize(2);
  cylinder_primitive.dimensions[primitive.CYLINDER_HEIGHT] = 0.20;
  cylinder_primitive.dimensions[primitive.CYLINDER_RADIUS] = 0.04;

  // We define the frame/pose for this cylinder so that it appears in the gripper.
  object_to_attach.header.frame_id = move_group.getEndEffectorLink();
  geometry_msgs::msg::Pose grab_pose;
  grab_pose.orientation.w = 1.0;
  grab_pose.position.z = 0.2;

  // First, we add the object to the world (without using a vector).
  object_to_attach.primitives.push_back(cylinder_primitive);
  object_to_attach.primitive_poses.push_back(grab_pose);
  object_to_attach.operation = object_to_attach.ADD;
  planning_scene_interface.applyCollisionObject(object_to_attach);

  // Then, we "attach" the object to the robot. It uses the frame_id to determine which robot link it is attached to.
  // We also need to tell MoveIt that the object is allowed to be in collision with the finger links of the gripper.
  // You could also use applyAttachedCollisionObject to attach an object to the robot directly.
  RCLCPP_INFO(LOGGER, "Attach the object to the robot");
  std::vector<std::string> touch_links;
  touch_links.push_back("panda_rightfinger");
  touch_links.push_back("panda_leftfinger");
  move_group.attachObject(object_to_attach.id, "panda_hand", touch_links);


 
   ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////


  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.7;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);



  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();

  if(success) 
   move_group.execute(my_plan);
  else 
   RCLCPP_INFO(LOGGER, "Planning failed!");

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
 

  ///////////////////////////////////////Task-space pose goal//////////////////////////////////////////////////////////////////////////////
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");


  target_pose.orientation.w = 0;
  target_pose.orientation.x = -1.0;
  target_pose.orientation.y = 0;
  target_pose.orientation.z = 0;
  target_pose.position.x = 0.28;
  target_pose.position.y = 0;
  target_pose.position.z = 0.59;
  move_group.setPoseTarget(target_pose);


  success = static_cast<bool>(move_group.plan(my_plan));
  visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
  visual_tools.trigger();
  if(success) {
    move_group.execute(my_plan);
  } else {
    RCLCPP_INFO(LOGGER, "Planning failed!");
  }

  visual_tools.deleteAllMarkers();
  visual_tools.trigger();



  // END_TUTORIAL
  rclcpp::shutdown();
  return 0;
}
