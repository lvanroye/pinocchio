//
// Copyright (c) 2015-2020 CNRS INRIA
//

#include "pinocchio/bindings/python/algorithm/algorithms.hpp"
#include "pinocchio/algorithm/frames.hpp"

namespace pinocchio
{
  namespace python
  {
    
    static context::Data::Matrix6x get_frame_jacobian_proxy1(const context::Model & model,
                                                             context::Data & data,
                                                             const context::Data::FrameIndex frame_id,
                                                             ReferenceFrame rf)
    {
      context::Data::Matrix6x J(6,model.nv); J.setZero();
      getFrameJacobian(model, data, frame_id, rf, J);
      
      return J;
    }
  
    static context::Data::Matrix6x get_frame_jacobian_proxy2(const context::Model & model,
                                                             context::Data & data,
                                                             const context::Data::JointIndex joint_id,
                                                             const context::SE3 & placement,
                                                             ReferenceFrame rf)
    {
      context::Data::Matrix6x J(6,model.nv); J.setZero();
      getFrameJacobian(model, data, joint_id, placement, rf, J);
      
      return J;
    }
    
    static context::Data::Matrix6x compute_frame_jacobian_proxy(const context::Model & model,
                                                                context::Data & data,
                                                                const context::VectorXs & q,
                                                                context::Data::FrameIndex frame_id)
    {
      context::Data::Matrix6x J(6,model.nv); J.setZero();
      computeFrameJacobian(model, data, q, frame_id, J);
  
      return J;
    }
    
    static context::Data::Matrix6x compute_frame_jacobian_proxy(const context::Model & model,
                                                                context::Data & data,
                                                                const context::VectorXs & q,
                                                                context::Data::FrameIndex frame_id,
                                                                ReferenceFrame reference_frame)
    {
      context::Data::Matrix6x J(6,model.nv); J.setZero();
      computeFrameJacobian(model, data, q, frame_id, reference_frame, J);
  
      return J;
    }

    static context::Data::Matrix6x get_frame_jacobian_time_variation_proxy(const context::Model & model,
                                                                           context::Data & data,
                                                                           context::Data::FrameIndex jointId,
                                                                           ReferenceFrame rf)
    {
      context::Data::Matrix6x dJ(6,model.nv); dJ.setZero();
      getFrameJacobianTimeVariation(model,data,jointId,rf,dJ);
      
      return dJ;
    }

    static context::Data::Matrix6x frame_jacobian_time_variation_proxy(const context::Model & model,
                                                                       context::Data & data,
                                                                       const context::VectorXs & q,
                                                                       const context::VectorXs & v,
                                                                       const context::Data::FrameIndex frame_id,
                                                                       const ReferenceFrame rf)
    {
      computeJointJacobiansTimeVariation(model,data,q,v);
      updateFramePlacements(model,data);
  
      return get_frame_jacobian_time_variation_proxy(model, data, frame_id, rf);
    }
    
    BOOST_PYTHON_FUNCTION_OVERLOADS(getFrameVelocity_overload, (getFrameVelocity<context::Scalar,context::Options,JointCollectionDefaultTpl>), 3, 4)
    BOOST_PYTHON_FUNCTION_OVERLOADS(getFrameAcceleration_overload, (getFrameAcceleration<context::Scalar,context::Options,JointCollectionDefaultTpl>), 3, 4)
    BOOST_PYTHON_FUNCTION_OVERLOADS(getFrameClassicalAcceleration_overload, (getFrameClassicalAcceleration<context::Scalar,context::Options,JointCollectionDefaultTpl>), 3, 4)

    void exposeFramesAlgo()
    {
      typedef context::Scalar Scalar;
      typedef context::VectorXs VectorXs;
      enum { Options = context::Options };
      
      bp::def("updateFramePlacements",
              &updateFramePlacements<Scalar,Options,JointCollectionDefaultTpl>,
              bp::args("model","data"),
              "Computes the placements of all the operational frames according to the current joint placement stored in data"
              "and puts the results in data.");

      bp::def("updateFramePlacement",
              &updateFramePlacement<Scalar,Options,JointCollectionDefaultTpl>,
              bp::args("model","data","frame_id"),
              "Computes the placement of the given operational frame (frame_id) according to the current joint placement stored in data, stores the results in data and returns it.",
              bp::return_value_policy<bp::return_by_value>());

      bp::def("getFrameVelocity",
              &getFrameVelocity<Scalar,Options,JointCollectionDefaultTpl>,
              getFrameVelocity_overload(
                bp::args("model","data","frame_id","reference_frame"),
                "Returns the spatial velocity of the frame expressed in the coordinate system given by reference_frame.\n"
                "forwardKinematics(model,data,q,v[,a]) should be called first to compute the joint spatial velocity stored in data.v"));

      bp::def("getFrameAcceleration",
              &getFrameAcceleration<Scalar,Options,JointCollectionDefaultTpl>,
              getFrameAcceleration_overload(
                bp::args("model","data","frame_id","reference_frame"),
                "Returns the spatial acceleration of the frame expressed in the coordinate system given by reference_frame.\n"
                "forwardKinematics(model,data,q,v,a) should be called first to compute the joint spatial acceleration stored in data.a ."));

      bp::def("getFrameClassicalAcceleration",
              &getFrameClassicalAcceleration<Scalar,Options,JointCollectionDefaultTpl>,
              getFrameClassicalAcceleration_overload(
                bp::args("model","data","frame_id","reference_frame"),
                "Returns the \"classical\" acceleration of the frame expressed in the coordinate system given by reference_frame.\n"
                "forwardKinematics(model,data,q,v,a) should be called first to compute the joint spatial acceleration stored in data.a ."));

      bp::def("framesForwardKinematics",
              &framesForwardKinematics<Scalar,Options,JointCollectionDefaultTpl,VectorXs>,
              bp::args("model","data","q"),
              "Calls first the forwardKinematics(model,data,q) and then update the Frame placement quantities (data.oMf).");
      
      bp::def("computeFrameJacobian",
              (context::Data::Matrix6x (*)(const context::Model &, context::Data &, const context::VectorXs &, context::Data::FrameIndex, ReferenceFrame))&compute_frame_jacobian_proxy,
              bp::args("model","data","q","frame_id","reference_frame"),
              "Computes the Jacobian of the frame given by its frame_id in the coordinate system given by reference_frame.\n");
      
      bp::def("computeFrameJacobian",
              (context::Data::Matrix6x (*)(const context::Model &, context::Data &, const context::VectorXs &, context::Data::FrameIndex))&compute_frame_jacobian_proxy,
              bp::args("model","data","q","frame_id"),
              "Computes the Jacobian of the frame given by its frame_id.\n"
              "The columns of the Jacobian are expressed in the coordinates system of the Frame itself.\n"
              "In other words, the velocity of the frame vF expressed in the local coordinate is given by J*v,"
              "where v is the joint velocity.");
      
      bp::def("getFrameJacobian",
              &get_frame_jacobian_proxy1,
              bp::args("model","data","frame_id","reference_frame"),
              "Computes the Jacobian of the frame given by its ID either in the LOCAL, LOCAL_WORLD_ALIGNED or the WORLD coordinates systems.\n"
              "In other words, the velocity of the frame vF expressed in the reference frame is given by J*v,"
              "where v is the joint velocity vector.\n"
              "remarks: computeJointJacobians(model,data,q) must have been called first.");
      
      bp::def("getFrameJacobian",
              &get_frame_jacobian_proxy2,
              bp::args("model","data","joint_id","placement","reference_frame"),
              "Computes the Jacobian of the frame given by its placement with respect to the Joint frame and expressed the solution either in the LOCAL, LOCAL_WORLD_ALIGNED or the WORLD coordinates systems.\n"
              "In other words, the velocity of the frame vF expressed in the reference frame is given by J*v,"
              "where v is the joint velocity vector.\n\n"
              "remarks: computeJointJacobians(model,data,q) must have been called first.");

      bp::def("frameJacobianTimeVariation",&frame_jacobian_time_variation_proxy,
              bp::args("model","data","q","v","frame_id","reference_frame"),
              "Computes the Jacobian Time Variation of the frame given by its frame_id either in the reference frame provided by reference_frame.\n");

      bp::def("getFrameJacobianTimeVariation",get_frame_jacobian_time_variation_proxy,
              bp::args("model","data","frame_id","reference_frame"),
              "Returns the Jacobian time variation of the frame given by its frame_id either in the reference frame provided by reference_frame.\n"
              "You have to call computeJointJacobiansTimeVariation(model,data,q,v) and updateFramePlacements(model,data) first.");

    }
  
  } // namespace python

} // namespace pinocchio
