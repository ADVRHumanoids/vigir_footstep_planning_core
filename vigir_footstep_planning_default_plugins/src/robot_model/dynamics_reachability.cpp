#include <vigir_footstep_planning_default_plugins/robot_model/dynamics_reachability.h>



namespace vigir_footstep_planning
{
DynamicsReachability::DynamicsReachability()
  : ReachabilityPlugin("dynamics_reachability")
{
}

bool DynamicsReachability::loadParams(const ParameterSet& global_params)
{
  if (!ReachabilityPlugin::loadParams(global_params))
    return false;

  global_params.getParam("dynamics/body/max_vel", max_body_vel_sq, 0.0);
  max_body_vel_sq *= max_body_vel_sq;
  global_params.getParam("dynamics/body/max_acc", max_body_acc_sq, 0.0);
  max_body_acc_sq *= max_body_acc_sq;
  return true;
}

bool DynamicsReachability::isReachable(const State& current, const State& next) const
{
  if (max_body_vel_sq > 0.0)
  {
    const geometry_msgs::Vector3& v = next.getBodyVelocity();
    if ((v.x*v.x + v.y*v.y + v.z*v.z) > max_body_vel_sq)
      return false;
  }

  if (max_body_acc_sq > 0.0 && next.getStepDuration() > 0.0)
  {
    const geometry_msgs::Vector3& v0 = current.getBodyVelocity();
    const geometry_msgs::Vector3& v1 = next.getBodyVelocity();

    geometry_msgs::Vector3 acc;
    acc.x = (v1.x-v0.x)/next.getStepDuration();
    acc.y = (v1.y-v0.y)/next.getStepDuration();
    acc.z = (v1.z-v0.z)/next.getStepDuration();

    if ((acc.x*acc.x + acc.y*acc.y + acc.z*acc.z) > max_body_acc_sq)
      return false;
  }

  return true;
}
}

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(vigir_footstep_planning::DynamicsReachability, vigir_footstep_planning::ReachabilityPlugin)
