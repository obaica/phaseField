#include "../../include/matrixFreePDE.h"

template <int dim, int degree>
class customPDE: public MatrixFreePDE<dim,degree>
{
public:
	customPDE(userInputParameters<dim> _userInputs): MatrixFreePDE<dim,degree>(_userInputs) , userInputs(_userInputs) {};

private:
	#include "../../include/typeDefs.h"

	const userInputParameters<dim> userInputs;

	// Pure virtual method in MatrixFreePDE
    void residualExplicitRHS(variableContainer<dim,degree,dealii::VectorizedArray<double> > & variable_list,
					 dealii::Point<dim, dealii::VectorizedArray<double> > q_point_loc) const;

    void residualNonexplicitRHS(variableContainer<dim,degree,dealii::VectorizedArray<double> > & variable_list,
					 dealii::Point<dim, dealii::VectorizedArray<double> > q_point_loc) const;

     // Pure virtual method in MatrixFreePDE
 	void residualLHS(variableContainer<dim,degree,dealii::VectorizedArray<double> > & variable_list,
 					 dealii::Point<dim, dealii::VectorizedArray<double> > q_point_loc) const;


	// Virtual method in MatrixFreePDE that we override if we need postprocessing
	#ifdef POSTPROCESS_FILE_EXISTS
	void postProcessedFields(const variableContainer<dim,degree,dealii::VectorizedArray<double> > & variable_list,
					variableContainer<dim,degree,dealii::VectorizedArray<double> > & pp_variable_list,
					const dealii::Point<dim, dealii::VectorizedArray<double> > q_point_loc) const;
	#endif

	// Virtual method in MatrixFreePDE that we override if we need nucleation
	#ifdef NUCLEATION_FILE_EXISTS
	double getNucleationProbability(variableValueContainer variable_value, double dV) const;
	#endif

	// ================================================================
	// Methods specific to this subclass
	// ================================================================


	// ================================================================
	// Model constants specific to this subclass
	// ================================================================

	double Mc = userInputs.get_model_constant_double("Mc");
	double Mn = userInputs.get_model_constant_double("Mn");
	double Kn = userInputs.get_model_constant_double("Kn");

    dealii::Tensor<1,dim> center1 = userInputs.get_model_constant_rank_1_tensor("center1");
    dealii::Tensor<1,dim> center2 = userInputs.get_model_constant_rank_1_tensor("center2");

    double radius1 = userInputs.get_model_constant_double("radius1");
    double radius2 = userInputs.get_model_constant_double("radius2");

    double matrix_concentration = userInputs.get_model_constant_double("matrix_concentration");


	// ================================================================

};
