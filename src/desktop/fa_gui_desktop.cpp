#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "fa_sm.h"
#include "fa_sm_commands.h"
#include "fa_led.h"
#include "fa_error.h"
#include "fa_ota.h"

static GLFWwindow *window = NULL;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
extern uint8_t desktop_sensor_error;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void GuiInit()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    window = glfwCreateWindow(1100, 680, "Frankair Sim", NULL, NULL);
    if (window == NULL)
        return;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    bool show_demo_window = true;
    bool show_another_window = false;
}

bool GuiShouldClose()
{
    return glfwWindowShouldClose(window);
}

bool SliderUint8(const char *label, uint8_t *v, uint8_t v_min, uint8_t v_max)
{
    int val = *v;
    bool ret = ImGui::SliderInt(label, &val, v_min, v_max);
    *v = (uint8_t)val;
    return ret;
}
bool SliderUint16(const char *label, uint16_t *v, uint16_t v_min, uint16_t v_max)
{
    int val = *v;
    bool ret = ImGui::SliderInt(label, &val, v_min, v_max);
    *v = (uint16_t)val;
    return ret;
}

void GuiUpdate()
{
    glfwPollEvents();

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static int counter = 0;
        bool open;
        // ImGui::Begin("Frankair", &open, ImGuiWindowFlags_NoTitleBar);
        ImGui::Begin("Sensor");

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("temp_exhaust_in", &override.temp.exhaust_in, -10.0f, 25.0f);
        ImGui::SliderFloat("hum_rel_exhaust_in", &override.humidity_rel_exhaust_in, 0.0f, 100.0f);
        ImGui::SliderFloat("temp_fresh_out", &override.temp.fresh_out, -10.0f, 25.0f);
        ImGui::SliderFloat("hum_rel_fresh_out", &override.humidity_rel_fresh_out, 0.0f, 100.0f);
        ImGui::SliderFloat("temp_fresh_in", &override.temp.fresh_in, -10.0f, 25.0f);
        ImGui::SliderFloat("temp_exhaust_out", &override.temp.exhaust_out, -10.0f, 25.0f);
        ImGui::Checkbox("Override enabled", &override.enabled);
        ImGui::Checkbox("isOnline", &state.is_online);
        SliderUint8("desktop_sensor_error", &desktop_sensor_error, 0, 32);
        ImGui::Checkbox("ota downloading", &ota.downloading);

        // state.humidity.abs_delta = state.humidity.abs_exhaust_in - state.humidity.abs_fresh_out;

        if (ImGui::Button("Sniff start"))
            smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_AUTO_SNIFF);
        ImGui::SameLine();
        if (ImGui::Button("Sniff finished"))
            smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SNIFF_FINISHED);
        ImGui::SameLine();
        if (ImGui::Button("Wait"))
            smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_AUTO_WAIT);
        ImGui::SameLine();
        if (ImGui::Button("Update"))
            smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_UPDATE);
        if (ImGui::Button("Clear Error"))
            errorClear();
        ImGui::SameLine();
        if (ImGui::Button("Settings changed"))
            cmdSettingsChanged();
        ImGui::SameLine();
        if (ImGui::Button("Reboot"))
            cmdReboot();
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
    {
        ImGui::Begin("Settings");
        ImGui::Checkbox("Enable abs humidity conditions", &settings.ctrl.simple.abs_min_enabled);
        ImGui::SliderFloat("ctrl.simple.abs_hum_min_on", &settings.ctrl.simple.abs_hum_min_on, 0, 3);
        ImGui::SliderFloat("ctrl.simple.abs_hum_min_wait", &settings.ctrl.simple.abs_hum_min_wait, 0, 3);
        ImGui::Checkbox("Enable rel humidity conditions", &settings.ctrl.simple.rel_min_enabled);
        SliderUint8("ctrl.simple.rel_min_wait", &settings.ctrl.simple.rel_min_wait, 0, 100);
        ImGui::Checkbox("Stop fan if room temp < desired min temp and fresh temp below room temp", &settings.ctrl.simple.desired_temp_min_enabled);
        SliderUint8("ctrl.desired_temp_min", &settings.ctrl.simple.desired_temp_min, 0, 30);
        ImGui::Checkbox("Fan max level if room temp < desired max temp and fresh temp > room temp", &settings.ctrl.simple.desired_temp_max_enabled);
        SliderUint8("ctrl.desired_temp_max", &settings.ctrl.simple.desired_temp_max, 0, 30);

        SliderUint8("sniff.sniff_sec", &settings.sniff.sniff_sec, 1, 60);
        SliderUint16("sniff.wait_sec", &settings.sniff.wait_sec, 1, 60);
        SliderUint8("controller_interval_sec", &settings.controller_interval_sec, 0, 60);
        SliderUint8("sensor_read_interval_sec", &settings.sensor_read_interval_sec, 0, 60);
        SliderUint8("stream_interval_min", &settings.stream_interval_min, 0, 60);
        ImGui::SliderFloat("measurement_alpha_sniff", &settings.measurement_alpha_sniff, 0, 1);
        ImGui::SliderFloat("measurement_alpha_on", &settings.measurement_alpha_on, 0, 1);
        SliderUint8("manual.level_fan_exhaust", &settings.manual.level_fan_exhaust, FAN_LEVEL_OFF, FAN_LEVEL_MAX);
        SliderUint8("manual.level_fan_fresh", &settings.manual.level_fan_fresh, FAN_LEVEL_OFF, FAN_LEVEL_MAX);
        SliderUint8("manual.level_fan_frost", &settings.manual.level_fan_frost, FAN_LEVEL_OFF, FAN_LEVEL_MAX);
        SliderUint8("manual.open_flap_frost", &settings.manual.open_flap_frost, FAN_LEVEL_OFF, FAN_LEVEL_MAX);
        ImGui::Checkbox("fa_frost_flap_ctrl_t.enabled", &settings.ctrl.frost_flap_ctrl.enabled);
        ImGui::SliderFloat("frost_flap_ctrl.temp_min_close", &settings.ctrl.frost_flap_ctrl.temp_min_close, -10, 10);
        ImGui::SliderFloat("frost_flap_ctrl.temp_min_open", &settings.ctrl.frost_flap_ctrl.temp_min_open, -10, 10);
        SliderUint8("frost_flap_ctrl.level_open", &settings.ctrl.frost_flap_ctrl.level_open, FLAP_LEVEL_CLOSE, FLAP_LEVEL_OPEN);

        ImGui::Checkbox("humidity_fan_curve", &settings.ctrl.humidity_fan_curve.enabled);
        ImGui::Checkbox("temp_fan_curve", &settings.ctrl.temp_fan_curve.enabled);
        int mode_current = settings.mode;
        ImGui::Combo("Mode", &mode_current, "off\0auto\0manual\0\0");
        settings.mode = mode_current;

        ImGui::End();
    }

    {
        ImGui::Begin("State");
        ImGui::DragScalar("hum abs_delta", ImGuiDataType_Float, &state.humidity.abs_delta);
        ImGui::DragScalar("hum abs_exhaust_in", ImGuiDataType_Float, &state.humidity.abs_exhaust_in);
        ImGui::DragScalar("hum abs_fresh_out", ImGuiDataType_Float, &state.humidity.abs_fresh_out);
        ImGui::DragScalar("temp exhaust_in", ImGuiDataType_Float, &state.temp.exhaust_in);
        ImGui::DragScalar("hum rel_exhaust_in", ImGuiDataType_Float, &state.humidity.rel_exhaust_in);
        ImGui::DragScalar("temp fresh_out", ImGuiDataType_Float, &state.temp.fresh_out);
        ImGui::DragScalar("hum rel_fresh_out", ImGuiDataType_Float, &state.humidity.rel_fresh_out);
        ImGui::DragScalar("temp fresh_in", ImGuiDataType_Float, &state.temp.fresh_in);
        ImGui::DragScalar("temp exhaust_out", ImGuiDataType_Float, &state.temp.exhaust_out);

        ImGui::ProgressBar(state.actuator.level_fan_exhaust / 10.0f, {0, 10});
        ImGui::SameLine();
        ImGui::Text("fan exhaust");
        ImGui::ProgressBar(state.actuator.level_fan_fresh / 10.0f, {0, 10});
        ImGui::SameLine();
        ImGui::Text("fan fresh");
        ImGui::ProgressBar(state.actuator.level_fan_frost / 10.0f, {0, 10});
        ImGui::SameLine();
        ImGui::Text("fan frost");
        ImGui::ProgressBar(state.actuator.open_flap_frost / 10.0f, {0, 10});
        ImGui::SameLine();
        ImGui::Text("flap frost");
        int val = state.sm_state;
        ImGui::Combo("state", &val, "root\0auto\0on\0sniff\0wait\0error\0manual\0off\0\0");
        ImGui::Checkbox("led", &led_on);

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

void GuiCleanup()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
