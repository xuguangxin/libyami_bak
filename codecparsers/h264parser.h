/* Gstreamer
 * Copyright (C) <2011> Intel Corporation
 * Copyright (C) <2011> Collabora Ltd.
 * Copyright (C) <2011> Thibault Saunier <thibault.saunier@collabora.com>
 *
 * Some bits C-c,C-v'ed and s/4/3 from h264parse and videoparsers/h264parse.c:
 *    Copyright (C) <2010> Mark Nauwelaerts <mark.nauwelaerts@collabora.co.uk>
 *    Copyright (C) <2010> Collabora Multimedia
 *    Copyright (C) <2010> Nokia Corporation
 *
 *    (C) 2005 Michal Benes <michal.benes@itonis.tv>
 *    (C) 2008 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __H264_PARSER_H__
#define __H264_PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "basictype.h"

#define H264_MAX_SPS_COUNT   32
#define H264_MAX_PPS_COUNT   256
#define H264_MAX_VIEW_COUNT  1024
#define H264_MAX_VIEW_ID     (H264_MAX_VIEW_COUNT - 1)

#define H264_IS_P_SLICE(slice)  (((slice)->type % 5) == H264_P_SLICE)
#define H264_IS_B_SLICE(slice)  (((slice)->type % 5) == H264_B_SLICE)
#define H264_IS_I_SLICE(slice)  (((slice)->type % 5) == H264_I_SLICE)
#define H264_IS_SP_SLICE(slice) (((slice)->type % 5) == H264_SP_SLICE)
#define H264_IS_SI_SLICE(slice) (((slice)->type % 5) == H264_SI_SLICE)

#define H264_IS_MVC_NALU(nalu) \
  ((nalu)->extension_type == H264_NAL_EXTENSION_MVC)

/**
 * @H264_PROFILE_BASELINE: Baseline profile (A.2.1)
 * @H264_PROFILE_MAIN: Main profile (A.2.2)
 * @H264_PROFILE_EXTENDED: Extended profile (A.2.3)
 * @H264_PROFILE_HIGH: High profile (A.2.4)
 * @H264_PROFILE_HIGH10: High 10 profile (A.2.5) or High 10 Intra
 *   profile (A.2.8), depending on constraint_set3_flag
 * @H264_PROFILE_HIGH_422: High 4:2:2 profile (A.2.6) or High
 *   4:2:2 Intra profile (A.2.9), depending on constraint_set3_flag
 * @H264_PROFILE_HIGH_444: High 4:4:4 Predictive profile (A.2.7)
 *   or High 4:4:4 Intra profile (A.2.10), depending on the value of
 *   constraint_set3_flag
 * @H264_PROFILE_MULTIVIEW_HIGH: Multiview High profile (H.10.1.1)
 * @H264_PROFILE_STEREO_HIGH: Stereo High profile (H.10.1.2)
 * @H264_PROFILE_SCALABLE_BASELINE: Scalable Baseline profile (G.10.1.1)
 * @H264_PROFILE_SCALABLE_HIGH: Scalable High profile (G.10.1.2)
 *   or Scalable High Intra profile (G.10.1.3), depending on the value
 *   of constraint_set3_flag
 *
 * H.264 Profiles.
 *
 * Since: 1.2
 */
typedef enum {
  H264_PROFILE_BASELINE             = 66,
  H264_PROFILE_MAIN                 = 77,
  H264_PROFILE_EXTENDED             = 88,
  H264_PROFILE_HIGH                 = 100,
  H264_PROFILE_HIGH10               = 110,
  H264_PROFILE_HIGH_422             = 122,
  H264_PROFILE_HIGH_444             = 244,
  H264_PROFILE_MULTIVIEW_HIGH       = 118,
  H264_PROFILE_STEREO_HIGH          = 128,
  H264_PROFILE_SCALABLE_BASELINE    = 83,
  H264_PROFILE_SCALABLE_HIGH        = 86
} H264Profile;

/**
 * H264NalUnitType:
 * @H264_NAL_UNKNOWN: Unknown nal type
 * @H264_NAL_SLICE: Slice nal
 * @H264_NAL_SLICE_DPA: DPA slice nal
 * @H264_NAL_SLICE_DPB: DPB slice nal
 * @H264_NAL_SLICE_DPC: DPC slice nal
 * @H264_NAL_SLICE_IDR: DPR slice nal
 * @H264_NAL_SEI: Supplemental enhancement information (SEI) nal unit
 * @H264_NAL_SPS: Sequence parameter set (SPS) nal unit
 * @H264_NAL_PPS: Picture parameter set (PPS) nal unit
 * @H264_NAL_AU_DELIMITER: Access unit (AU) delimiter nal unit
 * @H264_NAL_SEQ_END: End of sequence nal unit
 * @H264_NAL_STREAM_END: End of stream nal unit
 * @H264_NAL_FILLER_DATA: Filler data nal lunit
 * @H264_NAL_PREFIX_UNIT: Prefix NAL unit
 * @H264_NAL_SLICE_AUX: Auxiliary coded picture without partitioning NAL unit
 * @H264_NAL_SLICE_EXT: Coded slice extension NAL unit
 *
 * Indicates the type of H264 Nal Units
 */
typedef enum
{
  H264_NAL_UNKNOWN      = 0,
  H264_NAL_SLICE        = 1,
  H264_NAL_SLICE_DPA    = 2,
  H264_NAL_SLICE_DPB    = 3,
  H264_NAL_SLICE_DPC    = 4,
  H264_NAL_SLICE_IDR    = 5,
  H264_NAL_SEI          = 6,
  H264_NAL_SPS          = 7,
  H264_NAL_PPS          = 8,
  H264_NAL_AU_DELIMITER = 9,
  H264_NAL_SEQ_END      = 10,
  H264_NAL_STREAM_END   = 11,
  H264_NAL_FILLER_DATA  = 12,
  H264_NAL_SPS_EXT      = 13,
  H264_NAL_PREFIX_UNIT  = 14,
  H264_NAL_SUBSET_SPS   = 15,
  H264_NAL_SLICE_AUX    = 19,
  H264_NAL_SLICE_EXT    = 20
} H264NalUnitType;

/**
 * H264NalUnitExtensionType:
 * @H264_NAL_EXTENSION_NONE: No NAL unit header extension is available
 * @H264_NAL_EXTENSION_MVC: NAL unit header extension for MVC (Annex H)
 *
 * Indicates the type of H.264 NAL unit extension.
 */
typedef enum
{
  H264_NAL_EXTENSION_NONE = 0,
  H264_NAL_EXTENSION_MVC,
} H264NalUnitExtensionType;

/**
 * H264ParserResult:
 * @H264_PARSER_OK: The parsing succeded
 * @H264_PARSER_BROKEN_DATA: The data to parse is broken
 * @H264_PARSER_BROKEN_LINK: The link to structure needed for the parsing couldn't be found
 * @H264_PARSER_ERROR: An error accured when parsing
 * @H264_PARSER_NO_NAL: No nal found during the parsing
 * @H264_PARSER_NO_NAL_END: Start of the nal found, but not the end.
 *
 * The result of parsing H264 data.
 */
typedef enum
{
  H264_PARSER_OK,
  H264_PARSER_BROKEN_DATA,
  H264_PARSER_BROKEN_LINK,
  H264_PARSER_ERROR,
  H264_PARSER_NO_NAL,
  H264_PARSER_NO_NAL_END
} H264ParserResult;

/**
 * H264SEIPayloadType:
 * @H264_SEI_BUF_PERIOD: Buffering Period SEI Message
 * @H264_SEI_PIC_TIMING: Picture Timing SEI Message
 * ...
 *
 * The type of SEI message.
 */
typedef enum
{
  H264_SEI_BUF_PERIOD = 0,
  H264_SEI_PIC_TIMING = 1
      /* and more...  */
} H264SEIPayloadType;

/**
 * H264SEIPicStructType:
 * @H264_SEI_PIC_STRUCT_FRAME: Picture is a frame
 * @H264_SEI_PIC_STRUCT_TOP_FIELD: Top field of frame
 * @H264_SEI_PIC_STRUCT_BOTTOM_FIELD: Botom field of frame
 * @H264_SEI_PIC_STRUCT_TOP_BOTTOM: Top bottom field of frame
 * @H264_SEI_PIC_STRUCT_BOTTOM_TOP: bottom top field of frame
 * @H264_SEI_PIC_STRUCT_TOP_BOTTOM_TOP: top bottom top field of frame
 * @H264_SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM: bottom top bottom field of frame
 * @H264_SEI_PIC_STRUCT_FRAME_DOUBLING: indicates that the frame should
 *  be displayed two times consecutively
 * @H264_SEI_PIC_STRUCT_FRAME_TRIPLING: indicates that the frame should be
 *  displayed three times consecutively
 *
 * SEI pic_struct type
 */
typedef enum
{
  H264_SEI_PIC_STRUCT_FRAME             = 0,
  H264_SEI_PIC_STRUCT_TOP_FIELD         = 1,
  H264_SEI_PIC_STRUCT_BOTTOM_FIELD      = 2,
  H264_SEI_PIC_STRUCT_TOP_BOTTOM        = 3,
  H264_SEI_PIC_STRUCT_BOTTOM_TOP        = 4,
  H264_SEI_PIC_STRUCT_TOP_BOTTOM_TOP    = 5,
  H264_SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM = 6,
  H264_SEI_PIC_STRUCT_FRAME_DOUBLING    = 7,
  H264_SEI_PIC_STRUCT_FRAME_TRIPLING    = 8
} H264SEIPicStructType;

/**
 * H264SliceType:
 *
 * Type of Picture slice
 */

typedef enum
{
  H264_P_SLICE    = 0,
  H264_B_SLICE    = 1,
  H264_I_SLICE    = 2,
  H264_SP_SLICE   = 3,
  H264_SI_SLICE   = 4,
  H264_S_P_SLICE  = 5,
  H264_S_B_SLICE  = 6,
  H264_S_I_SLICE  = 7,
  H264_S_SP_SLICE = 8,
  H264_S_SI_SLICE = 9
} H264SliceType;

typedef struct _H264NalParser              H264NalParser;

typedef struct _H264NalUnit                H264NalUnit;
typedef struct _H264NalUnitExtensionMVC    H264NalUnitExtensionMVC;

typedef struct _H264SPSExtMVCView          H264SPSExtMVCView;
typedef struct _H264SPSExtMVCLevelValue    H264SPSExtMVCLevelValue;
typedef struct _H264SPSExtMVCLevelValueOp  H264SPSExtMVCLevelValueOp;
typedef struct _H264SPSExtMVC              H264SPSExtMVC;

typedef struct _H264SPS                    H264SPS;
typedef struct _H264PPS                    H264PPS;
typedef struct _H264HRDParams              H264HRDParams;
typedef struct _H264VUIParams              H264VUIParams;

typedef struct _H264RefPicListModification H264RefPicListModification;
typedef struct _H264DecRefPicMarking       H264DecRefPicMarking;
typedef struct _H264RefPicMarking          H264RefPicMarking;
typedef struct _H264PredWeightTable        H264PredWeightTable;
typedef struct _H264SliceHdr               H264SliceHdr;

typedef struct _H264ClockTimestamp         H264ClockTimestamp;
typedef struct _H264PicTiming              H264PicTiming;
typedef struct _H264BufferingPeriod        H264BufferingPeriod;
typedef struct _H264SEIMessage             H264SEIMessage;

/**
 * H264NalUnitExtensionMVC:
 * @non_idr_flag: If equal to 0, it specifies that the current access
 *   unit is an IDR access unit
 * @priority_id: The priority identifier for the NAL unit
 * @view_id: The view identifier for the NAL unit
 * @temporal_id: The temporal identifier for the NAL unit
 * @anchor_pic_flag: If equal to 1, it specifies that the current
 *   access unit is an anchor access unit
 * @inter_view_flag: If equal to 0, it specifies that the current view
 *   component is not used for inter-view prediction by any other view
 *   component in the current access unit
 */
struct _H264NalUnitExtensionMVC
{
  uint8 non_idr_flag;
  uint8 priority_id;
  uint16 view_id;
  uint8 temporal_id;
  uint8 anchor_pic_flag;
  uint8 inter_view_flag;
};

/**
 * H264NalUnit:
 * @ref_idc: not equal to 0 specifies that the content of the NAL unit contains a sequence
 *  parameter set, a sequence * parameter set extension, a subset sequence parameter set, a
 *  picture parameter set, a slice of a reference picture, a slice data partition of a
 *  reference picture, or a prefix NAL unit preceding a slice of a reference picture.
 * @type: A #H264NalUnitType
 * @idr_pic_flag: calculated idr_pic_flag
 * @size: The size of the nal unit starting from @offset
 * @offset: The offset of the actual start of the nal unit
 * @sc_offset:The offset of the start code of the nal unit
 * @valid: If the nal unit is valid, which mean it has
 * already been parsed
 * @data: The data from which the Nalu has been parsed
 * @header_bytes: The size of the NALU header in bytes
 *
 * Structure defining the Nal unit headers
 */
struct _H264NalUnit
{
  uint16 ref_idc;
  uint16 type;

  /* calculated values */
  uint8 idr_pic_flag;
  uint32 size;
  uint32 offset;
  uint32 sc_offset;
  boolean valid;

  uint8 *data;

  uint8 header_bytes;
  uint8 extension_type;
  union {
    H264NalUnitExtensionMVC mvc;
  } extension;
};

/**
 * H264HRDParams:
 * @cpb_cnt_minus1: plus 1 specifies the number of alternative
 *    CPB specifications in the bitstream
 * @bit_rate_scale: specifies the maximum input bit rate of the
 * SchedSelIdx-th CPB
 * @cpb_size_scale: specifies the CPB size of the SchedSelIdx-th CPB
 * @uint32 bit_rate_value_minus1: specifies the maximum input bit rate for the
 * SchedSelIdx-th CPB
 * @cpb_size_value_minus1: is used together with cpb_size_scale to specify the
 * SchedSelIdx-th CPB size
 * @cbr_flag: Specifies if running in itermediate bitrate mode or constant
 * @initial_cpb_removal_delay_length_minus1: specifies the length in bits of
 * the cpb_removal_delay syntax element
 * @cpb_removal_delay_length_minus1: specifies the length in bits of the
 * dpb_output_delay syntax element
 * @dpb_output_delay_length_minus1: >0 specifies the length in bits of the time_offset syntax element.
 * =0 specifies that the time_offset syntax element is not present
 * @time_offset_length: Length of the time offset
 *
 * Defines the HRD parameters
 */
struct _H264HRDParams
{
  uint8 cpb_cnt_minus1;
  uint8 bit_rate_scale;
  uint8 cpb_size_scale;

  uint32 bit_rate_value_minus1[32];
  uint32 cpb_size_value_minus1[32];
  uint8  cbr_flag[32];

  uint8 initial_cpb_removal_delay_length_minus1;
  uint8 cpb_removal_delay_length_minus1;
  uint8 dpb_output_delay_length_minus1;
  uint8 time_offset_length;
};

/**
 * H264VUIParams:
 * @aspect_ratio_info_present_flag: %TRUE specifies that aspect_ratio_idc is present.
 *  %FALSE specifies that aspect_ratio_idc is not present
 * @aspect_ratio_idc specifies the value of the sample aspect ratio of the luma samples
 * @sar_width indicates the horizontal size of the sample aspect ratio
 * @sar_height indicates the vertical size of the sample aspect ratio
 * @overscan_info_present_flag: %TRUE overscan_appropriate_flag is present %FALSE otherwize
 * @overscan_appropriate_flag: %TRUE indicates that the cropped decoded pictures
 *  output are suitable for display using overscan. %FALSE the cropped decoded pictures
 *  output contain visually important information
 * @video_signal_type_present_flag: %TRUE specifies that video_format, video_full_range_flag and
 *  colour_description_present_flag are present.
 * @video_format: indicates the representation of the picture
 * @video_full_range_flag: indicates the black level and range of the luma and chroma signals
 * @colour_description_present_flag: %TRUE specifies that colour_primaries,
 *  transfer_characteristics and matrix_coefficients are present
 * @colour_primaries: indicates the chromaticity coordinates of the source primaries
 * @transfer_characteristics: indicates the opto-electronic transfer characteristic
 * @matrix_coefficients: describes the matrix coefficients used in deriving luma and chroma signals
 * @chroma_loc_info_present_flag: %TRUE specifies that chroma_sample_loc_type_top_field and
 *  chroma_sample_loc_type_bottom_field are present, %FALSE otherwize
 * @chroma_sample_loc_type_top_field: specify the location of chroma for top field
 * @chroma_sample_loc_type_bottom_field specify the location of chroma for bottom field
 * @timing_info_present_flag: %TRUE specifies that num_units_in_tick,
 *  time_scale and fixed_frame_rate_flag are present in the bitstream
 * @num_units_in_tick: is the number of time units of a clock operating at the frequency time_scale Hz
 * time_scale: is the number of time units that pass in one second
 * @fixed_frame_rate_flag: %TRUE indicates that the temporal distance between the HRD output times
 *  of any two consecutive pictures in output order is constrained as specified in the spec, %FALSE
 *  otherwize.
 * @nal_hrd_parameters_present_flag: %TRUE if nal hrd parameters present in the bitstream
 * @vcl_hrd_parameters_present_flag: %TRUE if nal vlc hrd parameters present in the bitstream
 * @low_delay_hrd_flag: specifies the HRD operational mode
 * @pic_struct_present_flag: %TRUE specifies that picture timing SEI messages are present or not
 * @bitstream_restriction_flag: %TRUE specifies that the following coded video sequence bitstream restriction
 * parameters are present
 * @motion_vectors_over_pic_boundaries_flag: %FALSE indicates that no sample outside the
 *  picture boundaries and no sample at a fractional sample position, %TRUE indicates that one or more
 *  samples outside picture boundaries may be used in inter prediction
 * @max_bytes_per_pic_denom: indicates a number of bytes not exceeded by the sum of the sizes of
 *  the VCL NAL units associated with any coded picture in the coded video sequence.
 * @max_bits_per_mb_denom: indicates the maximum number of coded bits of macroblock_layer
 * @log2_max_mv_length_horizontal: indicate the maximum absolute value of a decoded horizontal
 * motion vector component
 * @log2_max_mv_length_vertical: indicate the maximum absolute value of a decoded vertical
 *  motion vector component
 * @num_reorder_frames: indicates the maximum number of frames, complementary field pairs,
 *  or non-paired fields that precede any frame,
 * @max_dec_frame_buffering: specifies the required size of the HRD decoded picture buffer in
 *  units of frame buffers.
 *
 * The structure representing the VUI parameters.
 */
struct _H264VUIParams
{
  uint8 aspect_ratio_info_present_flag;
  uint8 aspect_ratio_idc;
  /* if aspect_ratio_idc == 255 */
  uint16 sar_width;
  uint16 sar_height;

  uint8 overscan_info_present_flag;
  /* if overscan_info_present_flag */
  uint8 overscan_appropriate_flag;

  uint8 video_signal_type_present_flag;
  uint8 video_format;
  uint8 video_full_range_flag;
  uint8 colour_description_present_flag;
  uint8 colour_primaries;
  uint8 transfer_characteristics;
  uint8 matrix_coefficients;

  uint8 chroma_loc_info_present_flag;
  uint8 chroma_sample_loc_type_top_field;
  uint8 chroma_sample_loc_type_bottom_field;

  uint8 timing_info_present_flag;
  /* if timing_info_present_flag */
  uint32 num_units_in_tick;
  uint32 time_scale;
  uint8 fixed_frame_rate_flag;

  uint8 nal_hrd_parameters_present_flag;
  /* if nal_hrd_parameters_present_flag */
  H264HRDParams nal_hrd_parameters;

  uint8 vcl_hrd_parameters_present_flag;
  /* if nal_hrd_parameters_present_flag */
  H264HRDParams vcl_hrd_parameters;

  uint8 low_delay_hrd_flag;
  uint8 pic_struct_present_flag;

  uint8 bitstream_restriction_flag;
  /*  if bitstream_restriction_flag */
  uint8 motion_vectors_over_pic_boundaries_flag;
  uint32 max_bytes_per_pic_denom;
  uint32 max_bits_per_mb_denom;
  uint32 log2_max_mv_length_horizontal;
  uint32 log2_max_mv_length_vertical;
  uint32 num_reorder_frames;
  uint32 max_dec_frame_buffering;

  /* calculated values */
  uint32 par_n;
  uint32 par_d;
};

/**
 * H264SPSExtMVCView:
 * @num_anchor_refs_l0: specifies the number of view components for
 *   inter-view prediction in the initialized RefPicList0 in decoding
 *   anchor view components.
 * @anchor_ref_l0: specifies the view_id for inter-view prediction in
 *   the initialized RefPicList0 in decoding anchor view components.
 * @num_anchor_refs_l1: specifies the number of view components for
 *   inter-view prediction in the initialized RefPicList1 in decoding
 *   anchor view components.
 * @anchor_ref_l1: specifies the view_id for inter-view prediction in
 *   the initialized RefPicList1 in decoding anchor view components.
 * @num_non_anchor_refs_l0: specifies the number of view components
 *   for inter-view prediction in the initialized RefPicList0 in
 *   decoding non-anchor view components.
 * @non_anchor_ref_l0: specifies the view_id for inter-view prediction
 *   in the initialized RefPicList0 in decoding non-anchor view
 *   components.
 * @num_non_anchor_refs_l1: specifies the number of view components
 *   for inter-view prediction in the initialized RefPicList1 in
 *   decoding non-anchor view components.
 * @non_anchor_ref_l1: specifies the view_id for inter-view prediction
 *   in the initialized RefPicList1 in decoding non-anchor view
 *   components.
 *
 * Represents inter-view dependency relationships for the coded video
 * sequence.
 */
struct _H264SPSExtMVCView
{
  uint16 view_id;
  uint8 num_anchor_refs_l0;
  uint16 anchor_ref_l0[15];
  uint8 num_anchor_refs_l1;
  uint16 anchor_ref_l1[15];
  uint8 num_non_anchor_refs_l0;
  uint16 non_anchor_ref_l0[15];
  uint8 num_non_anchor_refs_l1;
  uint16 non_anchor_ref_l1[15];
};

/**
 * H264SPSExtMVCLevelValueOp:
 *
 * Represents an operation point for the coded video sequence.
 */
struct _H264SPSExtMVCLevelValueOp
{
  uint8 temporal_id;
  uint16 num_target_views_minus1;
  uint16 *target_view_id;
  uint16 num_views_minus1;
};

/**
 * H264SPSExtMVCLevelValue:
 * @level_idc: specifies the level value signalled for the coded video
 *   sequence
 * @num_applicable_ops_minus1: plus 1 specifies the number of
 *   operation points to which the level indicated by level_idc applies
 * @applicable_op: specifies the applicable operation point
 *
 * Represents level values for a subset of the operation points for
 * the coded video sequence.
 */
struct _H264SPSExtMVCLevelValue
{
  uint8 level_idc;
  uint16 num_applicable_ops_minus1;
  H264SPSExtMVCLevelValueOp *applicable_op;
};

/**
 * H264SPSExtMVC:
 * @num_views_minus1: plus 1 specifies the maximum number of coded
 *   views in the coded video sequence
 * @view: array of #H264SPSExtMVCView
 * @num_level_values_signalled_minus1: plus 1 specifies the number of
 *   level values signalled for the coded video sequence.
 * @level_value: array of #H264SPSExtMVCLevelValue
 *
 * Represents the parsed seq_parameter_set_mvc_extension().
 */
struct _H264SPSExtMVC
{
  uint16 num_views_minus1;
  H264SPSExtMVCView *view;
  uint8 num_level_values_signalled_minus1;
  H264SPSExtMVCLevelValue *level_value;
};

/**
 * H264SPS:
 * @id: The ID of the sequence parameter set
 * @profile_idc: indicate the profile to which the coded video sequence conforms
 *
 * H264 Sequence Parameter Set (SPS)
 */
struct _H264SPS
{
  int32 id;

  uint8 profile_idc;
  uint8 constraint_set0_flag;
  uint8 constraint_set1_flag;
  uint8 constraint_set2_flag;
  uint8 constraint_set3_flag;
  uint8 constraint_set4_flag;
  uint8 constraint_set5_flag;
  uint8 level_idc;

  uint8 chroma_format_idc;
  uint8 separate_colour_plane_flag;
  uint8 bit_depth_luma_minus8;
  uint8 bit_depth_chroma_minus8;
  uint8 qpprime_y_zero_transform_bypass_flag;

  uint8 scaling_matrix_present_flag;
  uint8 scaling_lists_4x4[6][16];
  uint8 scaling_lists_8x8[6][64];

  uint8 log2_max_frame_num_minus4;
  uint8 pic_order_cnt_type;

  /* if pic_order_cnt_type == 0 */
  uint8 log2_max_pic_order_cnt_lsb_minus4;

  /* else if pic_order_cnt_type == 1 */
  uint8 delta_pic_order_always_zero_flag;
  int32 offset_for_non_ref_pic;
  int32 offset_for_top_to_bottom_field;
  uint8 num_ref_frames_in_pic_order_cnt_cycle;
  int32 offset_for_ref_frame[255];

  uint32 num_ref_frames;
  uint8 gaps_in_frame_num_value_allowed_flag;
  uint32 pic_width_in_mbs_minus1;
  uint32 pic_height_in_map_units_minus1;
  uint8 frame_mbs_only_flag;

  uint8 mb_adaptive_frame_field_flag;

  uint8 direct_8x8_inference_flag;

  uint8 frame_cropping_flag;

  /* if frame_cropping_flag */
  uint32 frame_crop_left_offset;
  uint32 frame_crop_right_offset;
  uint32 frame_crop_top_offset;
  uint32 frame_crop_bottom_offset;

  uint8 vui_parameters_present_flag;
  /* if vui_parameters_present_flag */
 H264VUIParams vui_parameters;

  /* calculated values */
  uint8 chroma_array_type;
  uint32 max_frame_num;
  int32 width, height;
  int32 fps_num, fps_den;
  boolean valid;

  /* Subset SPS extensions */
  uint8 extension_type;
  union {
    H264SPSExtMVC mvc;
  } extension;
};

/**
 * H264PPS:
 *
 * H264 Picture Parameter Set
 */
struct _H264PPS
{
  int32 id;

  H264SPS *sequence;

  uint8 entropy_coding_mode_flag;
  uint8 pic_order_present_flag;

  uint32 num_slice_groups_minus1;

  /* if num_slice_groups_minus1 > 0 */
  uint8 slice_group_map_type;
  /* and if slice_group_map_type == 0 */
  uint32 run_length_minus1[8];
  /* or if slice_group_map_type == 2 */
  uint32 top_left[8];
  uint32 bottom_right[8];
  /* or if slice_group_map_type == (3, 4, 5) */
  uint8 slice_group_change_direction_flag;
  uint32 slice_group_change_rate_minus1;
  /* or if slice_group_map_type == 6 */
  uint32 pic_size_in_map_units_minus1;
  uint8 *slice_group_id;

  uint8 num_ref_idx_l0_active_minus1;
  uint8 num_ref_idx_l1_active_minus1;
  uint8 weighted_pred_flag;
  uint8 weighted_bipred_idc;
  int8 pic_init_qp_minus26;
  int8 pic_init_qs_minus26;
  int8 chroma_qp_index_offset;
  uint8 deblocking_filter_control_present_flag;
  uint8 constrained_intra_pred_flag;
  uint8 redundant_pic_cnt_present_flag;

  uint8 transform_8x8_mode_flag;

  uint8 scaling_lists_4x4[6][16];
  uint8 scaling_lists_8x8[6][64];

  uint8 second_chroma_qp_index_offset;

  boolean valid;
};

struct _H264RefPicListModification
{
  uint8 modification_of_pic_nums_idc;
  union
  {
    /* if modification_of_pic_nums_idc == 0 || 1 */
    uint32 abs_diff_pic_num_minus1;
    /* if modification_of_pic_nums_idc == 2 */
    uint32 long_term_pic_num;
    /* if modification_of_pic_nums_idc == 4 || 5 */
    uint32 abs_diff_view_idx_minus1;
  } value;
};

struct _H264PredWeightTable
{
  uint8 luma_log2_weight_denom;
  uint8 chroma_log2_weight_denom;

  int16 luma_weight_l0[32];
  int8 luma_offset_l0[32];

  /* if seq->ChromaArrayType != 0 */
  int16 chroma_weight_l0[32][2];
  int8 chroma_offset_l0[32][2];

  /* if slice->slice_type % 5 == 1 */
  int16 luma_weight_l1[32];
  int8 luma_offset_l1[32];

  /* and if seq->ChromaArrayType != 0 */
  int16 chroma_weight_l1[32][2];
  int8 chroma_offset_l1[32][2];
};

struct _H264RefPicMarking
{
  uint8 memory_management_control_operation;

  uint32 difference_of_pic_nums_minus1;
  uint32 long_term_pic_num;
  uint32 long_term_frame_idx;
  uint32 max_long_term_frame_idx_plus1;
};

struct _H264DecRefPicMarking
{
  /* if slice->nal_unit.IdrPicFlag */
  uint8 no_output_of_prior_pics_flag;
  uint8 long_term_reference_flag;

  uint8 adaptive_ref_pic_marking_mode_flag;
  H264RefPicMarking ref_pic_marking[10];
  uint8 n_ref_pic_marking;
};


struct _H264SliceHdr
{
  uint32 first_mb_in_slice;
  uint32 type;
  H264PPS *pps;

  /* if seq->separate_colour_plane_flag */
  uint8 colour_plane_id;

  uint16 frame_num;

  uint8 field_pic_flag;
  uint8 bottom_field_flag;

  /* if nal_unit.type == 5 */
  uint16 idr_pic_id;

  /* if seq->pic_order_cnt_type == 0 */
  uint16 pic_order_cnt_lsb;
  /* if seq->pic_order_present_flag && !field_pic_flag */
  int32 delta_pic_order_cnt_bottom;

  int32 delta_pic_order_cnt[2];
  uint8 redundant_pic_cnt;

  /* if slice_type == B_SLICE */
  uint8 direct_spatial_mv_pred_flag;

  uint8 num_ref_idx_l0_active_minus1;
  uint8 num_ref_idx_l1_active_minus1;

  uint8 ref_pic_list_modification_flag_l0;
  uint8 n_ref_pic_list_modification_l0;
  H264RefPicListModification ref_pic_list_modification_l0[32];
  uint8 ref_pic_list_modification_flag_l1;
  uint8 n_ref_pic_list_modification_l1;
  H264RefPicListModification ref_pic_list_modification_l1[32];

  H264PredWeightTable pred_weight_table;
  /* if nal_unit.ref_idc != 0 */
  H264DecRefPicMarking dec_ref_pic_marking;

  uint8 cabac_init_idc;
  int8 slice_qp_delta;
  int8 slice_qs_delta;

  uint8 disable_deblocking_filter_idc;
  int8 slice_alpha_c0_offset_div2;
  int8 slice_beta_offset_div2;

  uint16 slice_group_change_cycle;

  /* calculated values */
  uint32 max_pic_num;
  boolean valid;

  /* Size of the slice_header() in bits */
  uint32 header_size;

  /* Number of emulation prevention bytes (EPB) in this slice_header() */
  uint32 n_emulation_prevention_bytes;

  /* slice nal header bytes */
  uint32 nal_header_bytes;
};


struct _H264ClockTimestamp
{
  uint8 ct_type;
  uint8 nuit_field_based_flag;
  uint8 counting_type;
  uint8 discontinuity_flag;
  uint8 cnt_dropped_flag;
  uint8 n_frames;

  uint8 seconds_flag;
  uint8 seconds_value;

  uint8 minutes_flag;
  uint8 minutes_value;

  uint8 hours_flag;
  uint8 hours_value;

  uint32 time_offset;
};

struct _H264PicTiming
{
  uint32 cpb_removal_delay;
  uint32 dpb_output_delay;

  uint8 pic_struct_present_flag;
  /* if pic_struct_present_flag */
  uint8 pic_struct;

  uint8 clock_timestamp_flag[3];
  H264ClockTimestamp clock_timestamp[3];
};

struct _H264BufferingPeriod
{
  H264SPS *sps;

  /* seq->vui_parameters->nal_hrd_parameters_present_flag */
  uint8 nal_initial_cpb_removal_delay[32];
  uint8 nal_initial_cpb_removal_delay_offset[32];

  /* seq->vui_parameters->vcl_hrd_parameters_present_flag */
  uint8 vcl_initial_cpb_removal_delay[32];
  uint8 vcl_initial_cpb_removal_delay_offset[32];
};

struct _H264SEIMessage
{
  H264SEIPayloadType payloadType;

  union {
    H264BufferingPeriod buffering_period;
    H264PicTiming pic_timing;
    /* ... could implement more */
  } payload;
};

/**
 * H264NalParser:
 *
 * H264 NAL Parser (opaque structure).
 */
struct _H264NalParser
{
  /*< private >*/
  H264SPS sps[H264_MAX_SPS_COUNT];
  H264PPS pps[H264_MAX_PPS_COUNT];
  H264SPS *last_sps;
  H264PPS *last_pps;
};

H264NalParser *h264_nal_parser_new             (void);

H264ParserResult h264_parser_identify_nalu     (H264NalParser *nalparser,
                                                       const uint8 *data, uint32 offset,
                                                       size_t size, H264NalUnit *nalu);

H264ParserResult h264_parser_identify_nalu_unchecked (H264NalParser *nalparser,
                                                       const uint8 *data, uint32 offset,
                                                       size_t size, H264NalUnit *nalu);

H264ParserResult h264_parser_identify_nalu_avc (H264NalParser *nalparser, const uint8 *data,
                                                       uint32 offset, size_t size, uint8 nal_length_size,
                                                       H264NalUnit *nalu);

H264ParserResult h264_parser_parse_nal         (H264NalParser *nalparser,
                                                       H264NalUnit *nalu);

H264ParserResult h264_parser_parse_slice_hdr   (H264NalParser *nalparser, H264NalUnit *nalu,
                                                       H264SliceHdr *slice, boolean parse_pred_weight_table,
                                                       boolean parse_dec_ref_pic_marking);

H264ParserResult h264_parser_parse_sps         (H264NalParser *nalparser, H264NalUnit *nalu,
                                                       H264SPS *sps, boolean parse_vui_params);

H264ParserResult h264_parser_parse_subset_sps  (H264NalParser *nalparser, H264NalUnit *nalu,
                                                       H264SPS *sps, boolean parse_vui_params);

H264ParserResult h264_parser_parse_pps         (H264NalParser *nalparser,
                                                       H264NalUnit *nalu, H264PPS *pps);

H264ParserResult h264_parser_parse_sei         (H264NalParser *nalparser,
                                                       H264NalUnit *nalu, H264SEIMessage *sei);

void h264_nal_parser_free                      (H264NalParser *nalparser);

H264ParserResult h264_parse_sps                (H264NalUnit *nalu,
                                                       H264SPS *sps, boolean parse_vui_params);

H264ParserResult h264_parse_subset_sps         (H264NalUnit *nalu,
                                                       H264SPS *sps, boolean parse_vui_params);

H264ParserResult h264_parse_pps                (H264NalParser *nalparser,
                                                       H264NalUnit *nalu, H264PPS *pps);

boolean          h264_sps_copy                 (H264SPS * dst_sps,
                                                       const H264SPS * src_sps);

void             h264_sps_free_1               (H264SPS *sps);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
