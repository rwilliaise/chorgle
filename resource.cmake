
find_program(
	resbuild
	NAMES resbuild
	HINTS ${install_dir}
	NO_SYSTEM_ENVIRONMENT_PATH
)

function(add_resources target)
	set(result)
	foreach(in_f ${ARGN})
		file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
		get_filename_component(resource_name ${src_f} NAME)
		set(out_f "${CMAKE_SOURCE_DIR}/res/${resource_name}.h")
		add_custom_command(OUTPUT ${out_f}
			COMMAND ${resbuild} -o ${out_f} ${src_f}
			DEPENDS "${src_f}" ${resbuild}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMENT "Packing object ${out_f}"
			VERBATIM
		)
		list(APPEND target_res_list ${out_f})
	endforeach()
	add_custom_target(${target} ALL
		DEPENDS ${target_res_list})
endfunction()
