"use client";
import { LoginForm } from "@/components/features/LoginForm/LoginForm";
import { AuthHeader } from "@/components/shared/AuthHeader/AuthHeader";
import { Card, Flex, Box } from "@chakra-ui/react";
import { Text, Button, Input } from "@chakra-ui/react";
export default function LoginPage() {
  return (
    <Flex height="100vh">
      <Flex direction="column" width="100%" alignItems="center">
        <AuthHeader />
        <Flex
          height="80%"
          margin="20px"
          width="100%"
          alignItems="center"
          direction="column"
        >
          <LoginForm />
        </Flex>
      </Flex>
    </Flex>
  );
}
