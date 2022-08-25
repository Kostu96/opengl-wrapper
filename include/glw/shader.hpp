#pragma once
#include <cstdint>

namespace glw {

    class Shader final
    {
    public:
        enum class Type {
            Vertex,
            Fragment
        };

        Shader() = default;
        ~Shader();

        void createFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        void createFromSource(const char* vertexShaderSource, const char* fragmentShaderSource);

        void bind() const;
        void unbind() const;
    private:
        uint32_t compileShader(const char* source, Type type) const;

        uint32_t m_id = static_cast<uint32_t>(-1);
    };

} // namespace glw
