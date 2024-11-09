import { Flex } from "@chakra-ui/react";

export default function AuthLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <Flex direction="column" height="100vh">
      {children}
    </Flex>
  );
}
