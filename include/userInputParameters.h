// Class to load in the user input from parameters.h and the variable definition
// part of equations.h

#ifndef INCLUDE_USERINPUTPARAMETERS_H_
#define INCLUDE_USERINPUTPARAMETERS_H_

#include "dealIIheaders.h"
#include "list_of_CIJ.h"
#include "../src/userInputParameters/getCIJMatrix.h"
#include "model_variables.h"
#include "varBCs.h"
#include "inputFileReader.h"
#include "varTypeEnums.h"
#include "variableAttributeLoader.h"

template <int dim>
class userInputParameters
{
private:
	// Method to create the list of time steps where the results should be output (called from loadInputParameters)
	std::vector<unsigned int> setOutputTimeSteps(const std::string outputSpacingType, unsigned int numberOfOutputs,
													const std::vector<unsigned int> & userGivenTimeStepList);

public:
	// Method to read the input parameters from a file and load them into the class member variables
	userInputParameters(inputFileReader & input_file_reader, dealii::ParameterHandler & parameter_handler, variableAttributeLoader variable_attributes);

	// Method to create the list of BCs from the user input strings (called from loadInputParameters)
	void load_BC_list(const std::vector<std::string> list_of_BCs);

	// Map linking the model constant name to its index
	std::unordered_map<std::string,unsigned int> model_constant_name_map;

	// Methods to access members of 'model_constant', one for each type (since one can't template based on return values)
	// These are really just wrappers for Boost's 'get' function
	double get_model_constant_double(const std::string constant_name) const {return boost::get<double>(model_constants[model_constant_name_map.at(constant_name)]);};
	int get_model_constant_int(const std::string constant_name) const {return boost::get<int>(model_constants[model_constant_name_map.at(constant_name)]);};
	bool get_model_constant_bool(const std::string constant_name) const {return boost::get<bool>(model_constants[model_constant_name_map.at(constant_name)]);};
	dealii::Tensor<1,dim> get_model_constant_rank_1_tensor(const std::string constant_name) const {return boost::get<dealii::Tensor<1,dim> >(model_constants[model_constant_name_map.at(constant_name)]);};
	dealii::Tensor<2,dim> get_model_constant_rank_2_tensor(const std::string constant_name) const {return boost::get<dealii::Tensor<2,dim> >(model_constants[model_constant_name_map.at(constant_name)]);};

	// Method to load in the variable attributes
	void loadVariableAttributes(variableAttributeLoader variable_attributes);

	// Meshing parameters
	std::vector<double> domain_size;
	std::vector<unsigned int> subdivisions;
	unsigned int refine_factor;
	unsigned int degree;

	// Mesh refinement parameters
	bool h_adaptivity;
	unsigned int max_refinement_level;
	unsigned int min_refinement_level;

	std::vector<int> refine_criterion_fields;
	std::vector<double> refine_window_max;
	std::vector<double> refine_window_min;

	unsigned int skip_remeshing_steps;

	// Output parameters
	unsigned int skip_print_steps;
	std::string output_file_type;
	std::string output_file_name;
	std::vector<unsigned int> outputTimeStepList;

	bool calc_energy;

	// Time step parameters
	double dtValue;
	double finalTime;
	unsigned int totalIncrements;

	// Elliptic solver parameters
	std::string solver_type;
	bool abs_tol;
	double solver_tolerance;
	unsigned int max_solver_iterations;

	// Variable inputs
	unsigned int number_of_variables;

	std::vector<std::string> var_name;
	std::vector<fieldType> var_type;
	std::vector<PDEType> var_eq_type;

	// Variables needed to calculate the LHS
	std::vector<variable_info> varInfoListRHS;

	// Variables needed to calculate the LHS
	unsigned int num_var_LHS;
	std::vector<variable_info> varInfoListLHS;

	// Variables for loading in initial conditions
	std::vector<bool> load_ICs;
	std::vector<bool> load_serial_file;
	std::vector<std::string> load_file_name;
	std::vector<std::string> load_field_name;

	// Elasticity tensor
	std::vector<dealii::Tensor<2, 2*dim-1+dim/3, dealii::VectorizedArray<double> > > CIJ_list;
	list_of_CIJ<dim> material_moduli;

	// Postprocessing parameters
	unsigned int pp_number_of_variables;
	unsigned int num_integrated_fields;
	bool postProcessingRequired;
	std::vector<bool> pp_calc_integral;
	std::vector<unsigned int> integrated_field_indices;

	std::vector<std::string> pp_var_name;
	std::vector<fieldType> pp_var_type;

	// Variable and residual info
	std::vector<variable_info> pp_varInfoList;
	std::vector<variable_info> pp_baseVarInfoList;

	// List of boundary conditions
	std::vector<varBCs<dim> > BC_list;

	// List of user-defined constants
	std::vector<boost::variant<double,int,bool,dealii::Tensor<1,dim>,dealii::Tensor<2,dim> > > model_constants;

	// Nucleation parameters
	bool nucleation_occurs;
	std::vector<unsigned int> nucleating_variable_indices;
	std::vector<unsigned int> nucleation_need_value;
	std::vector<double> nucleus_semiaxes;
	std::vector<double> order_parameter_freeze_semiaxes;
	double no_nucleation_border_thickness;
	double nucleus_hold_time;
	double min_distance_between_nuclei; // Only enforced for nuclei placed during the same time step
	double nucleation_order_parameter_cutoff;
	unsigned int steps_between_nucleation_attempts;

};

#endif /* INCLUDE_USERINPUTPARAMETERS_H_ */
