//
// Copyright (c) 2015-2019 CNRS INRIA
//

#include "pinocchio/bindings/python/algorithm/algorithms.hpp"
#include "pinocchio/algorithm/joint-configuration.hpp"

namespace pinocchio
{
  namespace python
  {

    static Eigen::VectorXd normalize_proxy(const Model & model,
                                           const Eigen::VectorXd & config)
    {
      Eigen::VectorXd q(config);
      normalize(model,q);
      return q;
    }
    
    static Eigen::VectorXd randomConfiguration_proxy(const Model & model)
    {
      return randomConfiguration(model);
    }

    bp::tuple dIntegrate_proxy(const Model & model,
                               const Eigen::VectorXd & q,
                               const Eigen::VectorXd & dq)
    {
      Eigen::MatrixXd J0(Eigen::MatrixXd::Zero(model.nv,model.nv));
      Eigen::MatrixXd J1(Eigen::MatrixXd::Zero(model.nv,model.nv));

      dIntegrate(model,q,dq,J0,ARG0);
      dIntegrate(model,q,dq,J1,ARG1);

      return bp::make_tuple(J0,J1);
    }

    Eigen::MatrixXd dIntegrate_arg_proxy(const Model & model,
                                         const Eigen::VectorXd & q,
                                         const Eigen::VectorXd & dq,
                                         const ArgumentPosition arg)
    {
      Eigen::MatrixXd J(Eigen::MatrixXd::Zero(model.nv,model.nv));
      
      dIntegrate(model,q,dq,J0,arg);
      
      return J;
    }
  
    void exposeJointsAlgo()
    {
      using namespace Eigen;
      
      bp::def("integrate",
              &integrate<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q (size model.nq)",
                       "Velocity v (size model.nv)"),
              "Integrate the model for a tangent vector during one unit time .");
      
      bp::enum_<ArgumentPosition>("ArgumentPosition")
        .value("ARG0",ARG0)
        .value("ARG1",ARG1)
        .value("ARG2",ARG2)
        .value("ARG3",ARG3)
        .value("ARG4",ARG4)
        ;

      bp::def("dIntegrate",
              &dIntegrate_arg_proxy,
              bp::args("Model",
                       "Joint configuration q (size model.nq)",
                       "Joint velocity v (size model.nv)"),
              "Computes the partial derivatives of integrate function with respect to the first "
              "and the second argument, and returns the two Jacobians as a tuple. ");

      bp::def("dIntegrate",
              &dIntegrate_proxy,
              bp::args("Model",
                       "Joint configuration q (size model.nq)",
                       "Joint velocity v (size model.nv)",
                       "arg (either ARG0 or ARG1)"),
              "Computes the partial derivatives of integrate function with respect to the first (arg == ARG0) "
              "or the second argument (arg == ARG1). ");

      bp::def("interpolate",
              &interpolate<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q1 (size model.nq)",
                       "Configuration q2 (size model.nq)",
                       "Double u"),
              "Interpolate the model between two configurations.");
      
      bp::def("difference",
              &difference<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q1 (size model.nq)",
                       "Configuration q2 (size model.nq)"),
              "Difference between two configurations, ie. the tangent vector that must be integrated during one unit time"
              "to go from q1 to q2");
      
      bp::def("squaredDistance",
              &squaredDistance<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q1 (size model.nq)",
                       "Configuration q2 (size model.nq)"),
              "Squared distance vector between two configurations.");
      
      bp::def("distance",
              &distance<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q1 (size model.nq)",
                       "Configuration q2 (size model.nq)"),
              "Distance between two configurations.");
      
      bp::def("randomConfiguration",
              &randomConfiguration_proxy,
              bp::arg("Model"),
              "Generate a random configuration in the bounds given by the lower and upper limits contained in model.");
      
      bp::def("randomConfiguration",
              &randomConfiguration<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Joint lower limits (size model.nq)",
                       "Joint upper limits (size model.nq)"),
              "Generate a random configuration in the bounds given by the Joint lower and upper limits arguments.");
      
      bp::def("neutral",
              &neutral<double,0,JointCollectionDefaultTpl>,
              bp::arg("Model"),
              "Returns the neutral configuration vector associated to the model.");
      
      bp::def("normalize",normalize_proxy,
              bp::args("Model",
                       "Configuration q (size model.nq)"),
              "return the configuration normalized ");
      
      bp::def("isSameConfiguration",
              &isSameConfiguration<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model",
                       "Configuration q1 (size model.nq)",
                       "Configuration q2 (size model.nq)",
                       "Precision"),
              "Return true if two configurations are equivalent");
    }
    
  } // namespace python
} // namespace pinocchio
