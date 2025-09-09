#pragma once

namespace AntiDebug {
    void Init();                     // Führt alle Checks aus
    void PatchIsDebuggerPresent();   // Patcht die API
    void HideFromDebugger();         // Versteckt den Thread
    void RunChecks();                // Führt alle weiteren Prüfungen durch
}
