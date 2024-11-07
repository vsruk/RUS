import { AuthHeader } from "@/components/shared/AuthHeader/AuthHeader";
import { Flex } from "@chakra-ui/react";

export default function AuthLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <Flex direction="column" height="100vh">
      <AuthHeader />
      {children}
    </Flex>
  );
}
