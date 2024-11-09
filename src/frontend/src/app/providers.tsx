"use client";

import { Provider } from "@/components/ui/provider";
import { GoogleOAuthProvider } from "@react-oauth/google";
import { SWRConfig } from "swr";

export function Providers({ children }: { children: React.ReactNode }) {
  return (
    <SWRConfig>
      <GoogleOAuthProvider clientId="815227853986-csi79rsg2mo4um9e0jho67tafs4ipe1s.apps.googleusercontent.com">
        <Provider>{children}</Provider>
      </GoogleOAuthProvider>
    </SWRConfig>
  );
}
