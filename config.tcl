source ../legup.tcl
set_project CycloneV SoCKit ARM_Simple_Hybrid_System


#set_accelerator_function "multiply"
set_accelerator_function "cvGetOptimalDFTSize"
#loop_pipeline "loop_swap"

#set_parameter DEBUG_INSERT_DEBUG_RTL 1
