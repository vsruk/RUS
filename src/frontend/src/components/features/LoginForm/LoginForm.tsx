import { InputGroup } from "@/components/ui/input-group";
import { Flex, Text, Input, Heading } from "@chakra-ui/react";

export const LoginForm = () => {
  return (
    <>
      <Flex
        borderRadius="10px"
        bg="gray"
        direction="column"
        alignItems="center"
        as="form"
        height="100%"
        width="60%"
      >
        <Heading>Login</Heading>
        <Flex
          direction="row"
          width="80%"
          justifyContent="space-between"
          alignItems="center"
        >
          <Text>e-mail:</Text>
          <Input type="email" width="80%" />
        </Flex>
      </Flex>
    </>
  );
};
