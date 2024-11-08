import type { Metadata } from "next";
import { Provider } from "@/components/ui/provider";
import * as React from "react";
import { GoogleOAuthProvider } from "@react-oauth/google";
export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body>
        <GoogleOAuthProvider clientId="815227853986-csi79rsg2mo4um9e0jho67tafs4ipe1s.apps.googleusercontent.com">
          <Provider>{children}</Provider>
        </GoogleOAuthProvider>
      </body>
    </html>
  );
}
