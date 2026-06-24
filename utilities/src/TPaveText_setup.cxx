#include <TPaveText.h>

void TK_PaveSet(TPaveText* pt) {
 // Set text properties
    pt->SetTextColor(kBlack);  // Set text color
    pt->SetFillColor(0);      // Set background to transparent
    pt->SetTextFont(42);      // Set font

    // Configure the border to keep it visible but without the shadow
    pt->SetBorderSize(1);     // Keep a thin border
    pt->SetLineColor(kBlack); // Set border color (black, for example)
    pt->SetLineWidth(2);      // Set the width of the border line

    // Remove the shadow by setting the fill style (avoid shadow effect)
    pt->SetFillStyle(0);      // Transparent fill without shadow

    // Draw the text box
    pt->Draw();
}
