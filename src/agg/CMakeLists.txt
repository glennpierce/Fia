SET(AGG_SRCS 

		# Make the headers appear in visual studio 
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_alpha_mask_u8.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_glyph_raster_bin.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_allocator.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_arc.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_gsv_text.h                  
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_converter.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_array.h                  
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_image_accessors.h           
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_gouraud.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_arrowhead.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_image_filters.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_gouraud_gray.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_basics.h                 
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_line_aa_basics.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_gouraud_rgba.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_bezier_arc.h             
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_math.h                      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_gradient.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_bitset_iterator.h        
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_blur.h                      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_math_stroke.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_gradient_alpha.h    
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_gradient_lut.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_bounding_rect.h          
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/ agg_path_length.h               
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_image_filter.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_bspline.h                
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_path_storage.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_image_filter_gray.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_clip_liang_barsky.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_path_storage_integer.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_image_filter_rgb.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_color_gray.h             
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pattern_filters_rgba.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_image_filter_rgba.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_color_rgba.h             
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_amask_adaptor.h     
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_interpolator_adaptor.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_config.h                 
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_gray.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_interpolator_linear.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_adaptor_vcgen.h    
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_rgb.h                
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_interpolator_persp.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_adaptor_vpgen.h     
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_rgb_packed.h         
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_interpolator_trans.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_bspline.h          
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_rgba.h               
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_pixfmt_transposer.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_pattern_gray.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_clip_polygon.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_cells_aa.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_pattern_rgb.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_clip_polyline.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_compound_aa.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_pattern_rgba.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_close_polygon.h     
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_outline.h        
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_solid.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_concat.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_outline_aa.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_span_subdiv_adaptor.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_contour.h          
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_scanline_aa.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_affine.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_curve.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rasterizer_sl_clip.h        
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_bilinear.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_dash.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_base.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_double_path.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_markers.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_marker.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_mclip.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_perspective.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_marker_adaptor.h    
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_outline_aa.h       
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_single_path.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_segmentator.h       
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_outline_image.h    
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_viewport.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_shorten_path.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_primitives.h       
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_trans_warp_magnifier.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_smooth_poly1.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_raster_text.h     
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_bspline.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_stroke.h            
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_renderer_scanline.h         
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_contour.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_transform.h         
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rendering_buffer.h          
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_dash.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_conv_unclose_polygon.h   
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rendering_buffer_dynarow.h  
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_markers_term.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_curves.h                 
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_rounded_rect.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_smooth_poly1.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_bin.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_stroke.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_dda_line.h               
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_boolean_algebra.h 
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vcgen_vertex_sequence.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_ellipse.h                
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_p.h               
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vertex_sequence.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_ellipse_bresenham.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_storage_aa.h       
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vpgen_clip_polygon.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_embedded_raster_fonts.h  
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_storage_bin.h      
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vpgen_clip_polyline.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_font_cache_manager.h     
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_scanline_u.h                
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_vpgen_segmentator.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_gamma_functions.h       
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_shorten_path.h
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_gamma_lut.h              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/include/agg_simul_eq.h

		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_arc.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_arrowhead.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_bezier_arc.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_bspline.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_curves.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_contour.cpp              
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_dash.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_markers_term.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_smooth_poly1.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_stroke.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vcgen_bspline.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_gsv_text.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_image_filters.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_line_aa_basics.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_line_profile_aa.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_rounded_rect.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_sqrt_tables.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_embedded_raster_fonts.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_trans_affine.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_trans_warp_magnifier.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_trans_single_path.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_trans_double_path.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vpgen_clip_polygon.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vpgen_clip_polyline.cpp
		${FreeImageAlgorithms_SOURCE_DIR}/Agg/src/agg_vpgen_segmentator.cpp	
)
