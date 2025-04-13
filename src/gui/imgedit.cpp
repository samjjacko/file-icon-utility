#include <wx/wx.h>
#include <wx/image.h>

void ApplyTintAndSave(const wxString& inputFilePath, const wxString& outputFilePath, int tintR, int tintG, int tintB) {
    // Load the image
    wxImage image;
    if (!image.LoadFile(inputFilePath)) {
        wxLogError("Failed to load image: %s", inputFilePath);
        return;
    }

    // Ensure the image has RGB data
    if (!image.HasAlpha()) {
        image.InitAlpha(); // Initialize alpha channel if not present
    }

    // Get image dimensions
    int width = image.GetWidth();
    int height = image.GetHeight();

    // Access the raw RGB data
    unsigned char* data = image.GetData();
    if (!data) {
        wxLogError("Failed to access image data.");
        return;
    }
    
    // Apply the tint
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3; // Each pixel has 3 bytes (R, G, B)
            data[index] = std::min(255, data[index] + tintR);     // Red
            data[index + 1] = std::min(255, data[index + 1] + tintG); // Green
            data[index + 2] = std::min(255, data[index + 2] + tintB); // Blue
        }
    }

    // Save the tinted image to a new file
    if (!image.SaveFile(outputFilePath)) {
        wxLogError("Failed to save tinted image: %s", outputFilePath);
        return;
    }

    wxLogMessage("Tinted image saved successfully: %s", outputFilePath);
}