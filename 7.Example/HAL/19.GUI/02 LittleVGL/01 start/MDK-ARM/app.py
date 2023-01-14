# Keil 文件导入

from os import path
import pyperclip

template_group = r'''
        <Group>
          <GroupName>%s</GroupName>
          <Files>
%s
          </Files>
        </Group>
'''

template_file = r'''
            <File>
              <FileName>%s</FileName>
              <FileType>1</FileType>
              <FilePath>%s</FilePath>
            </File>
'''.strip('\n')


def generate_filelist(filelist):
    content = []
    for filepath in filelist:
        filedir = path.dirname(filepath)
        filename = filepath[len(filedir)+1:]
        content.append(template_file % (filename, filepath))
    return '\n'.join(content)


def generate_group(groupname, filelist):
    return template_group % (groupname, filelist)


def generate_includelist(filelist):
    content = []
    for filepath in filelist:
        content.append(filepath if filepath.endswith('.h') == -1 else path.dirname(filepath))
    return ';'.join(set(content))


def convert_to_filelist(content):
    filelist = []
    for item in content.split('\n'):
        i = item.strip()
        if i == "" or i in filelist:
            continue
        filelist.append(i)
    return filelist


files_c_examples = r"""
..\..\Lib\lvgl\examples\anim\lv_example_anim_1.c
..\..\Lib\lvgl\examples\anim\lv_example_anim_2.c
..\..\Lib\lvgl\examples\anim\lv_example_anim_3.c
..\..\Lib\lvgl\examples\anim\lv_example_anim_timeline_1.c
..\..\Lib\lvgl\examples\assets\animimg001.c
..\..\Lib\lvgl\examples\assets\animimg002.c
..\..\Lib\lvgl\examples\assets\animimg003.c
..\..\Lib\lvgl\examples\assets\img_caret_down.c
..\..\Lib\lvgl\examples\assets\img_cogwheel_alpha16.c
..\..\Lib\lvgl\examples\assets\img_cogwheel_argb.c
..\..\Lib\lvgl\examples\assets\img_cogwheel_chroma_keyed.c
..\..\Lib\lvgl\examples\assets\img_cogwheel_indexed16.c
..\..\Lib\lvgl\examples\assets\img_cogwheel_rgb.c
..\..\Lib\lvgl\examples\assets\img_hand.c
..\..\Lib\lvgl\examples\assets\img_skew_strip.c
..\..\Lib\lvgl\examples\assets\img_star.c
..\..\Lib\lvgl\examples\assets\imgbtn_left.c
..\..\Lib\lvgl\examples\assets\imgbtn_mid.c
..\..\Lib\lvgl\examples\assets\imgbtn_right.c
..\..\Lib\lvgl\examples\assets\emoji\img_emoji_F617.c
..\..\Lib\lvgl\examples\event\lv_example_event_1.c
..\..\Lib\lvgl\examples\event\lv_example_event_2.c
..\..\Lib\lvgl\examples\event\lv_example_event_3.c
..\..\Lib\lvgl\examples\event\lv_example_event_4.c
..\..\Lib\lvgl\examples\get_started\lv_example_get_started_1.c
..\..\Lib\lvgl\examples\get_started\lv_example_get_started_2.c
..\..\Lib\lvgl\examples\get_started\lv_example_get_started_3.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_1.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_2.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_3.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_4.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_5.c
..\..\Lib\lvgl\examples\layouts\flex\lv_example_flex_6.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_1.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_2.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_3.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_4.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_5.c
..\..\Lib\lvgl\examples\layouts\grid\lv_example_grid_6.c
..\..\Lib\lvgl\examples\libs\bmp\lv_example_bmp_1.c
..\..\Lib\lvgl\examples\libs\ffmpeg\lv_example_ffmpeg_1.c
..\..\Lib\lvgl\examples\libs\ffmpeg\lv_example_ffmpeg_2.c
..\..\Lib\lvgl\examples\libs\freetype\lv_example_freetype_1.c
..\..\Lib\lvgl\examples\libs\gif\img_bulb_gif.c
..\..\Lib\lvgl\examples\libs\gif\lv_example_gif_1.c
..\..\Lib\lvgl\examples\libs\png\img_wink_png.c
..\..\Lib\lvgl\examples\libs\png\lv_example_png_1.c
..\..\Lib\lvgl\examples\libs\qrcode\lv_example_qrcode_1.c
..\..\Lib\lvgl\examples\libs\rlottie\lv_example_rlottie_1.c
..\..\Lib\lvgl\examples\libs\rlottie\lv_example_rlottie_2.c
..\..\Lib\lvgl\examples\libs\rlottie\lv_example_rlottie_approve.c
..\..\Lib\lvgl\examples\libs\sjpg\lv_example_sjpg_1.c
..\..\Lib\lvgl\examples\others\fragment\lv_example_fragment_1.c
..\..\Lib\lvgl\examples\others\fragment\lv_example_fragment_2.c
..\..\Lib\lvgl\examples\others\gridnav\lv_example_gridnav_1.c
..\..\Lib\lvgl\examples\others\gridnav\lv_example_gridnav_2.c
..\..\Lib\lvgl\examples\others\gridnav\lv_example_gridnav_3.c
..\..\Lib\lvgl\examples\others\gridnav\lv_example_gridnav_4.c
..\..\Lib\lvgl\examples\others\ime\lv_example_ime_pinyin_1.c
..\..\Lib\lvgl\examples\others\ime\lv_example_ime_pinyin_2.c
..\..\Lib\lvgl\examples\others\imgfont\lv_example_imgfont_1.c
..\..\Lib\lvgl\examples\others\monkey\lv_example_monkey_1.c
..\..\Lib\lvgl\examples\others\monkey\lv_example_monkey_2.c
..\..\Lib\lvgl\examples\others\monkey\lv_example_monkey_3.c
..\..\Lib\lvgl\examples\others\msg\lv_example_msg_1.c
..\..\Lib\lvgl\examples\others\msg\lv_example_msg_2.c
..\..\Lib\lvgl\examples\others\msg\lv_example_msg_3.c
..\..\Lib\lvgl\examples\others\snapshot\lv_example_snapshot_1.c
..\..\Lib\lvgl\examples\porting\lv_port_disp_template.c
..\..\Lib\lvgl\examples\porting\lv_port_fs_template.c
..\..\Lib\lvgl\examples\porting\lv_port_indev_template.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_1.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_2.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_3.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_4.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_5.c
..\..\Lib\lvgl\examples\scroll\lv_example_scroll_6.c
..\..\Lib\lvgl\examples\styles\lv_example_style_1.c
..\..\Lib\lvgl\examples\styles\lv_example_style_10.c
..\..\Lib\lvgl\examples\styles\lv_example_style_11.c
..\..\Lib\lvgl\examples\styles\lv_example_style_12.c
..\..\Lib\lvgl\examples\styles\lv_example_style_13.c
..\..\Lib\lvgl\examples\styles\lv_example_style_14.c
..\..\Lib\lvgl\examples\styles\lv_example_style_15.c
..\..\Lib\lvgl\examples\styles\lv_example_style_2.c
..\..\Lib\lvgl\examples\styles\lv_example_style_3.c
..\..\Lib\lvgl\examples\styles\lv_example_style_4.c
..\..\Lib\lvgl\examples\styles\lv_example_style_5.c
..\..\Lib\lvgl\examples\styles\lv_example_style_6.c
..\..\Lib\lvgl\examples\styles\lv_example_style_7.c
..\..\Lib\lvgl\examples\styles\lv_example_style_8.c
..\..\Lib\lvgl\examples\styles\lv_example_style_9.c
..\..\Lib\lvgl\examples\widgets\animimg\lv_example_animimg_1.c
..\..\Lib\lvgl\examples\widgets\arc\lv_example_arc_1.c
..\..\Lib\lvgl\examples\widgets\arc\lv_example_arc_2.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_1.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_2.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_3.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_4.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_5.c
..\..\Lib\lvgl\examples\widgets\bar\lv_example_bar_6.c
..\..\Lib\lvgl\examples\widgets\btn\lv_example_btn_1.c
..\..\Lib\lvgl\examples\widgets\btn\lv_example_btn_2.c
..\..\Lib\lvgl\examples\widgets\btn\lv_example_btn_3.c
..\..\Lib\lvgl\examples\widgets\btnmatrix\lv_example_btnmatrix_1.c
..\..\Lib\lvgl\examples\widgets\btnmatrix\lv_example_btnmatrix_2.c
..\..\Lib\lvgl\examples\widgets\btnmatrix\lv_example_btnmatrix_3.c
..\..\Lib\lvgl\examples\widgets\calendar\lv_example_calendar_1.c
..\..\Lib\lvgl\examples\widgets\canvas\lv_example_canvas_1.c
..\..\Lib\lvgl\examples\widgets\canvas\lv_example_canvas_2.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_1.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_2.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_3.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_4.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_5.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_6.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_7.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_8.c
..\..\Lib\lvgl\examples\widgets\chart\lv_example_chart_9.c
..\..\Lib\lvgl\examples\widgets\checkbox\lv_example_checkbox_1.c
..\..\Lib\lvgl\examples\widgets\checkbox\lv_example_checkbox_2.c
..\..\Lib\lvgl\examples\widgets\colorwheel\lv_example_colorwheel_1.c
..\..\Lib\lvgl\examples\widgets\dropdown\lv_example_dropdown_1.c
..\..\Lib\lvgl\examples\widgets\dropdown\lv_example_dropdown_2.c
..\..\Lib\lvgl\examples\widgets\dropdown\lv_example_dropdown_3.c
..\..\Lib\lvgl\examples\widgets\img\lv_example_img_1.c
..\..\Lib\lvgl\examples\widgets\img\lv_example_img_2.c
..\..\Lib\lvgl\examples\widgets\img\lv_example_img_3.c
..\..\Lib\lvgl\examples\widgets\img\lv_example_img_4.c
..\..\Lib\lvgl\examples\widgets\imgbtn\lv_example_imgbtn_1.c
..\..\Lib\lvgl\examples\widgets\keyboard\lv_example_keyboard_1.c
..\..\Lib\lvgl\examples\widgets\keyboard\lv_example_keyboard_2.c
..\..\Lib\lvgl\examples\widgets\label\lv_example_label_1.c
..\..\Lib\lvgl\examples\widgets\label\lv_example_label_2.c
..\..\Lib\lvgl\examples\widgets\label\lv_example_label_3.c
..\..\Lib\lvgl\examples\widgets\label\lv_example_label_4.c
..\..\Lib\lvgl\examples\widgets\label\lv_example_label_5.c
..\..\Lib\lvgl\examples\widgets\led\lv_example_led_1.c
..\..\Lib\lvgl\examples\widgets\line\lv_example_line_1.c
..\..\Lib\lvgl\examples\widgets\list\lv_example_list_1.c
..\..\Lib\lvgl\examples\widgets\list\lv_example_list_2.c
..\..\Lib\lvgl\examples\widgets\menu\lv_example_menu_1.c
..\..\Lib\lvgl\examples\widgets\menu\lv_example_menu_2.c
..\..\Lib\lvgl\examples\widgets\menu\lv_example_menu_3.c
..\..\Lib\lvgl\examples\widgets\menu\lv_example_menu_4.c
..\..\Lib\lvgl\examples\widgets\menu\lv_example_menu_5.c
..\..\Lib\lvgl\examples\widgets\meter\lv_example_meter_1.c
..\..\Lib\lvgl\examples\widgets\meter\lv_example_meter_2.c
..\..\Lib\lvgl\examples\widgets\meter\lv_example_meter_3.c
..\..\Lib\lvgl\examples\widgets\meter\lv_example_meter_4.c
..\..\Lib\lvgl\examples\widgets\msgbox\lv_example_msgbox_1.c
..\..\Lib\lvgl\examples\widgets\obj\lv_example_obj_1.c
..\..\Lib\lvgl\examples\widgets\obj\lv_example_obj_2.c
..\..\Lib\lvgl\examples\widgets\roller\lv_example_roller_1.c
..\..\Lib\lvgl\examples\widgets\roller\lv_example_roller_2.c
..\..\Lib\lvgl\examples\widgets\roller\lv_example_roller_3.c
..\..\Lib\lvgl\examples\widgets\slider\lv_example_slider_1.c
..\..\Lib\lvgl\examples\widgets\slider\lv_example_slider_2.c
..\..\Lib\lvgl\examples\widgets\slider\lv_example_slider_3.c
..\..\Lib\lvgl\examples\widgets\span\lv_example_span_1.c
..\..\Lib\lvgl\examples\widgets\spinbox\lv_example_spinbox_1.c
..\..\Lib\lvgl\examples\widgets\spinner\lv_example_spinner_1.c
..\..\Lib\lvgl\examples\widgets\switch\lv_example_switch_1.c
..\..\Lib\lvgl\examples\widgets\table\lv_example_table_1.c
..\..\Lib\lvgl\examples\widgets\table\lv_example_table_2.c
..\..\Lib\lvgl\examples\widgets\tabview\lv_example_tabview_1.c
..\..\Lib\lvgl\examples\widgets\tabview\lv_example_tabview_2.c
..\..\Lib\lvgl\examples\widgets\textarea\lv_example_textarea_1.c
..\..\Lib\lvgl\examples\widgets\textarea\lv_example_textarea_2.c
..\..\Lib\lvgl\examples\widgets\textarea\lv_example_textarea_3.c
..\..\Lib\lvgl\examples\widgets\tileview\lv_example_tileview_1.c
..\..\Lib\lvgl\examples\widgets\win\lv_example_win_1.c
"""

files_c_core = r"""
..\..\Lib\lvgl\src\core\lv_disp.c
..\..\Lib\lvgl\src\core\lv_event.c
..\..\Lib\lvgl\src\core\lv_group.c
..\..\Lib\lvgl\src\core\lv_indev.c
..\..\Lib\lvgl\src\core\lv_indev_scroll.c
..\..\Lib\lvgl\src\core\lv_obj.c
..\..\Lib\lvgl\src\core\lv_obj_class.c
..\..\Lib\lvgl\src\core\lv_obj_draw.c
..\..\Lib\lvgl\src\core\lv_obj_pos.c
..\..\Lib\lvgl\src\core\lv_obj_scroll.c
..\..\Lib\lvgl\src\core\lv_obj_style.c
..\..\Lib\lvgl\src\core\lv_obj_style_gen.c
..\..\Lib\lvgl\src\core\lv_obj_tree.c
..\..\Lib\lvgl\src\core\lv_refr.c
..\..\Lib\lvgl\src\core\lv_theme.c
"""

files_c_draw = r"""
..\..\Lib\lvgl\src\draw\lv_draw.c
..\..\Lib\lvgl\src\draw\lv_draw_arc.c
..\..\Lib\lvgl\src\draw\lv_draw_img.c
..\..\Lib\lvgl\src\draw\lv_draw_label.c
..\..\Lib\lvgl\src\draw\lv_draw_layer.c
..\..\Lib\lvgl\src\draw\lv_draw_line.c
..\..\Lib\lvgl\src\draw\lv_draw_mask.c
..\..\Lib\lvgl\src\draw\lv_draw_rect.c
..\..\Lib\lvgl\src\draw\lv_draw_transform.c
..\..\Lib\lvgl\src\draw\lv_draw_triangle.c
..\..\Lib\lvgl\src\draw\lv_img_buf.c
..\..\Lib\lvgl\src\draw\lv_img_cache.c
..\..\Lib\lvgl\src\draw\lv_img_decoder.c
..\..\Lib\lvgl\src\draw\arm2d\lv_gpu_arm2d.c
..\..\Lib\lvgl\src\draw\nxp\lv_gpu_nxp.c
..\..\Lib\lvgl\src\draw\nxp\pxp\lv_draw_pxp_blend.c
..\..\Lib\lvgl\src\draw\nxp\pxp\lv_gpu_nxp_pxp.c
..\..\Lib\lvgl\src\draw\nxp\pxp\lv_gpu_nxp_pxp_osa.c
..\..\Lib\lvgl\src\draw\nxp\vglite\lv_draw_vglite_arc.c
..\..\Lib\lvgl\src\draw\nxp\vglite\lv_draw_vglite_blend.c
..\..\Lib\lvgl\src\draw\nxp\vglite\lv_draw_vglite_rect.c
..\..\Lib\lvgl\src\draw\nxp\vglite\lv_gpu_nxp_vglite.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_arc.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_bg.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_composite.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_img.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_label.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_layer.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_line.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_mask.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_polygon.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_rect.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_stack_blur.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_texture_cache.c
..\..\Lib\lvgl\src\draw\sdl\lv_draw_sdl_utils.c
..\..\Lib\lvgl\src\draw\stm32_dma2d\lv_gpu_stm32_dma2d.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_arc.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_blend.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_dither.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_gradient.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_img.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_layer.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_letter.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_line.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_polygon.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_rect.c
..\..\Lib\lvgl\src\draw\sw\lv_draw_sw_transform.c
..\..\Lib\lvgl\src\draw\swm341_dma2d\lv_gpu_swm341_dma2d.c
"""

files_c_hal = r"""
..\..\Lib\lvgl\src\hal\lv_hal_disp.c
..\..\Lib\lvgl\src\hal\lv_hal_indev.c
..\..\Lib\lvgl\src\hal\lv_hal_tick.c
"""

files_c_layout = r"""
..\..\Lib\lvgl\src\layouts\flex\lv_flex.c
..\..\Lib\lvgl\src\layouts\grid\lv_grid.c
"""

files_c_lib = r"""
..\..\Lib\lvgl\src\libs\bmp\lv_bmp.c
..\..\Lib\lvgl\src\libs\ffmpeg\lv_ffmpeg.c
..\..\Lib\lvgl\src\libs\freetype\lv_freetype.c
..\..\Lib\lvgl\src\libs\fsdrv\lv_fs_fatfs.c
..\..\Lib\lvgl\src\libs\fsdrv\lv_fs_posix.c
..\..\Lib\lvgl\src\libs\fsdrv\lv_fs_stdio.c
..\..\Lib\lvgl\src\libs\fsdrv\lv_fs_win32.c
..\..\Lib\lvgl\src\libs\gif\gifdec.c
..\..\Lib\lvgl\src\libs\gif\lv_gif.c
..\..\Lib\lvgl\src\libs\png\lodepng.c
..\..\Lib\lvgl\src\libs\png\lv_png.c
..\..\Lib\lvgl\src\libs\qrcode\lv_qrcode.c
..\..\Lib\lvgl\src\libs\qrcode\qrcodegen.c
..\..\Lib\lvgl\src\libs\rlottie\lv_rlottie.c
..\..\Lib\lvgl\src\libs\sjpg\lv_sjpg.c
..\..\Lib\lvgl\src\libs\sjpg\tjpgd.c
"""

files_c_misc = r"""
..\..\Lib\lvgl\src\misc\lv_anim.c
..\..\Lib\lvgl\src\misc\lv_anim_timeline.c
..\..\Lib\lvgl\src\misc\lv_area.c
..\..\Lib\lvgl\src\misc\lv_async.c
..\..\Lib\lvgl\src\misc\lv_bidi.c
..\..\Lib\lvgl\src\misc\lv_color.c
..\..\Lib\lvgl\src\misc\lv_fs.c
..\..\Lib\lvgl\src\misc\lv_gc.c
..\..\Lib\lvgl\src\misc\lv_ll.c
..\..\Lib\lvgl\src\misc\lv_log.c
..\..\Lib\lvgl\src\misc\lv_lru.c
..\..\Lib\lvgl\src\misc\lv_malloc_builtin.c
..\..\Lib\lvgl\src\misc\lv_math.c
..\..\Lib\lvgl\src\misc\lv_mem.c
..\..\Lib\lvgl\src\misc\lv_memcpy_builtin.c
..\..\Lib\lvgl\src\misc\lv_printf.c
..\..\Lib\lvgl\src\misc\lv_style.c
..\..\Lib\lvgl\src\misc\lv_style_gen.c
..\..\Lib\lvgl\src\misc\lv_templ.c
..\..\Lib\lvgl\src\misc\lv_timer.c
..\..\Lib\lvgl\src\misc\lv_tlsf.c
..\..\Lib\lvgl\src\misc\lv_txt.c
..\..\Lib\lvgl\src\misc\lv_txt_ap.c
..\..\Lib\lvgl\src\misc\lv_utils.c
"""

files_c_other = r"""
..\..\Lib\lvgl\src\others\fragment\lv_fragment.c
..\..\Lib\lvgl\src\others\fragment\lv_fragment_manager.c
..\..\Lib\lvgl\src\others\gridnav\lv_gridnav.c
..\..\Lib\lvgl\src\others\ime\lv_ime_pinyin.c
..\..\Lib\lvgl\src\others\imgfont\lv_imgfont.c
..\..\Lib\lvgl\src\others\monkey\lv_monkey.c
..\..\Lib\lvgl\src\others\msg\lv_msg.c
"""

files_c_theme = r"""
..\..\Lib\lvgl\src\others\snapshot\lv_snapshot.c
..\..\Lib\lvgl\src\themes\basic\lv_theme_basic.c
..\..\Lib\lvgl\src\themes\default\lv_theme_default.c
..\..\Lib\lvgl\src\themes\mono\lv_theme_mono.c
"""

files_c_widgets = r"""
..\..\Lib\lvgl\src\widgets\animimg\lv_animimg.c
..\..\Lib\lvgl\src\widgets\arc\lv_arc.c
..\..\Lib\lvgl\src\widgets\bar\lv_bar.c
..\..\Lib\lvgl\src\widgets\btn\lv_btn.c
..\..\Lib\lvgl\src\widgets\btnmatrix\lv_btnmatrix.c
..\..\Lib\lvgl\src\widgets\calendar\lv_calendar.c
..\..\Lib\lvgl\src\widgets\calendar\lv_calendar_header_arrow.c
..\..\Lib\lvgl\src\widgets\calendar\lv_calendar_header_dropdown.c
..\..\Lib\lvgl\src\widgets\canvas\lv_canvas.c
..\..\Lib\lvgl\src\widgets\chart\lv_chart.c
..\..\Lib\lvgl\src\widgets\checkbox\lv_checkbox.c
..\..\Lib\lvgl\src\widgets\colorwheel\lv_colorwheel.c
..\..\Lib\lvgl\src\widgets\dropdown\lv_dropdown.c
..\..\Lib\lvgl\src\widgets\img\lv_img.c
..\..\Lib\lvgl\src\widgets\imgbtn\lv_imgbtn.c
..\..\Lib\lvgl\src\widgets\keyboard\lv_keyboard.c
..\..\Lib\lvgl\src\widgets\label\lv_label.c
..\..\Lib\lvgl\src\widgets\led\lv_led.c
..\..\Lib\lvgl\src\widgets\line\lv_line.c
..\..\Lib\lvgl\src\widgets\list\lv_list.c
..\..\Lib\lvgl\src\widgets\menu\lv_menu.c
..\..\Lib\lvgl\src\widgets\meter\lv_meter.c
..\..\Lib\lvgl\src\widgets\msgbox\lv_msgbox.c
..\..\Lib\lvgl\src\widgets\objx_templ\lv_objx_templ.c
..\..\Lib\lvgl\src\widgets\roller\lv_roller.c
..\..\Lib\lvgl\src\widgets\slider\lv_slider.c
..\..\Lib\lvgl\src\widgets\span\lv_span.c
..\..\Lib\lvgl\src\widgets\spinbox\lv_spinbox.c
..\..\Lib\lvgl\src\widgets\spinner\lv_spinner.c
..\..\Lib\lvgl\src\widgets\switch\lv_switch.c
..\..\Lib\lvgl\src\widgets\table\lv_table.c
..\..\Lib\lvgl\src\widgets\tabview\lv_tabview.c
..\..\Lib\lvgl\src\widgets\textarea\lv_textarea.c
..\..\Lib\lvgl\src\widgets\tileview\lv_tileview.c
..\..\Lib\lvgl\src\widgets\win\lv_win.c
"""

files_c_port = r"""
..\..\Lib\lvgl\porting\lv_port_disp.c
..\..\Lib\lvgl\porting\lv_port_fs.c
..\..\Lib\lvgl\porting\lv_port_indev.c
"""

files_c_font = r"""
..\..\Lib\lvgl\src\font\lv_font.c
..\..\Lib\lvgl\src\font\lv_font_dejavu_16_persian_hebrew.c
..\..\Lib\lvgl\src\font\lv_font_fmt_txt.c
..\..\Lib\lvgl\src\font\lv_font_loader.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_10.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_12.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_12_subpx.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_14.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_16.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_18.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_20.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_22.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_24.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_26.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_28.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_28_compressed.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_30.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_32.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_34.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_36.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_38.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_40.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_42.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_44.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_46.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_48.c
..\..\Lib\lvgl\src\font\lv_font_montserrat_8.c
..\..\Lib\lvgl\src\font\lv_font_simsun_16_cjk.c
..\..\Lib\lvgl\src\font\lv_font_unscii_16.c
..\..\Lib\lvgl\src\font\lv_font_unscii_8.c
"""

files_h = r"""
..\..\Lib\
..\..\Lib\lvgl\
""".strip()


output = '\n'.join(
    [
        generate_group("lvgl/examples", generate_filelist(convert_to_filelist(files_c_examples))),
        # generate_group("lvgl/core", generate_filelist(convert_to_filelist(files_c_core))),
        # generate_group("lvgl/draw", generate_filelist(convert_to_filelist(files_c_draw))),
        # generate_group("lvgl/hal", generate_filelist(convert_to_filelist(files_c_hal))),
        # generate_group("lvgl/layout", generate_filelist(convert_to_filelist(files_c_layout))),
        # generate_group("lvgl/lib", generate_filelist(convert_to_filelist(files_c_lib))),
        # generate_group("lvgl/widgets", generate_filelist(convert_to_filelist(files_c_widgets))),
        # generate_group("lvgl/port", generate_filelist(convert_to_filelist(files_c_port))),
        # generate_group("lvgl/font", generate_filelist(convert_to_filelist(files_c_font)))
        # generate_group("lvgl/misc", generate_filelist(convert_to_filelist(files_c_misc))),
        # generate_group("lvgl/other", generate_filelist(convert_to_filelist(files_c_other))),
        # generate_group("lvgl/theme", generate_filelist(convert_to_filelist(files_c_theme))),

    ]
)

# output = generate_includelist(convert_to_filelist(files_h)

pyperclip.copy(
    output
)
