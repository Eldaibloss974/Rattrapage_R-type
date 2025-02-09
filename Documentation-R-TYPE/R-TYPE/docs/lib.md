# Graphic Libraries

## Comparative Study

## Comparative Analysis of Graphics Libraries: SFML vs SDL2 vs Raylib

| Criteria | SFML | SDL2 | Raylib |
|----------|------|------|---------|
| **Release Year** | 2007 | 2013 (SDL2) | 2013 |
| **Primary Language** | C++ | C | C |
| **License** | zlib/png | zlib | zlib |
| **Memory Footprint** | ~2-3 MB | ~1-2 MB | ~800 KB |
| **External Dependencies** | OpenGL | Minimal | None |
| **2D Graphics Performance** | Good (60-120 FPS for 1000 sprites) | Excellent (100-150 FPS for 1000 sprites) | Very Good (80-130 FPS for 1000 sprites) |
| **3D Support** | No | Yes (via OpenGL/Direct3D) | Yes (native) |
| **Supported Platforms** | Windows, macOS, Linux, iOS, Android | Windows, macOS, Linux, iOS, Android, ConsoleOS | Windows, macOS, Linux, Android, Web (HTML5) |
| **Key Features** | - Hardware-accelerated 2D graphics<br>- Audio system<br>- Network module<br>- Window/Input management<br>- Shader support | - Low-level hardware access<br>- Video playback<br>- Multiple rendering backends<br>- Extensive input support<br>- Audio mixing | - No external dependencies<br>- Built-in math module<br>- Multiple language bindings<br>- 2D and 3D support<br>- Shader support |
| **Learning Curve** | Medium | Steep | Gentle |
| **Documentation** | Yes | Yes | Yes |
| **Community Size** | Large<br>(~15K GitHub stars) | Very Large<br>(~25K GitHub stars) | Growing<br>(~12K GitHub stars) |
| **Audio Formats** | OGG, WAV, FLAC | MP3, WAV, OGG, FLAC | WAV, OGG, MP3, FLAC |
| **Notable Projects** | - Crypt of the NecroDancer<br>- Risk of Rain | - Valve games<br>- Numerous Humble Bundle games | - Various indie games<br>- Educational projects |
| **Build Time** | Medium | Fast | Very Fast |
| **API Style** | Object-Oriented | Procedural | Procedural |
| **Resource Management** | Automatic | Manual | Semi-automatic |

### Performance Metrics (based on common benchmarks)
- **Window Creation Time**:
  - SFML: ~15-20ms
  - SDL2: ~10-15ms
  - Raylib: ~5-10ms

- **Memory Usage for Basic Window**:
  - SFML: ~5-8MB
  - SDL2: ~3-5MB
  - Raylib: ~2-4MB

- **Texture Loading (1024x1024)**:
  - SFML: ~20-25ms
  - SDL2: ~15-20ms
  - Raylib: ~10-15ms

Note: Performance metrics are approximate and may vary depending on hardware and implementation specifics.