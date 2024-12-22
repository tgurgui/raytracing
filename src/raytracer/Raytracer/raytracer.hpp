#pragma once
#include <vector>
#include <atomic>
#include <memory>

// fw declaration
class hittable_list;
class camera;

class Raytracer 
{
public:

    void trace(std::shared_ptr<hittable_list> scene, std::vector<unsigned char>& texture);
    unsigned int width() const {return m_width;}
    unsigned int height() const {return m_height;}
    unsigned int samplesPerPixel() const {return m_samples_per_pixel;}

    void setWidth(unsigned int width) {m_width = width;}
    void setHeight(unsigned int height) {m_height = height;}
    void setSamplesPerPixel(unsigned int samples_per_pixel) {m_samples_per_pixel = samples_per_pixel;}
    void stopRendering()
    {
        m_is_rendering = false;
    }

    bool isRendering()
    {
        return m_is_rendering;
    }

private:
    unsigned int m_width{1200}, m_height{675}, m_samples_per_pixel{5};
    std::atomic<bool> m_is_rendering{ false };  // Atomic flag to indicate is rendering
    std::shared_ptr<camera> m_camera {nullptr};
};

std::shared_ptr<camera> final_render_camera();
hittable_list random_scene();
void write_ppm(const std::string& filename, const std::vector<unsigned char>& pixels, unsigned int width, unsigned int height);
