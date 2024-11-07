import { InputGroup } from "@/components/ui/input-group";
import { Flex, Text, Input, Heading, Button } from "@chakra-ui/react";

export const LoginForm = () => {
  return (
    <>
      <Flex
        padding="20px"
        borderRadius="10px"
        bg="gray.300"
        direction="column"
        alignItems="center"
        as="form"
        height="100%"
        width="60%"
        gapY="10px"
      >
        <Heading marginBottom="20px" fontSize="2xl">
          Login
        </Heading>
        <Flex
          direction="row"
          width="80%"
          justifyContent="space-between"
          alignItems="center"
        >
          <Text>e-mail:</Text>
          <Input type="email" width="80%" borderColor="black" />
        </Flex>
        <Flex
          direction="row"
          width="80%"
          justifyContent="space-between"
          alignItems="center"
        >
          <Text>Lozinka:</Text>
          <Input type="password" width="80%" borderColor="black" />
        </Flex>
        <Button color="white" bg="gray" marginTop="20px" width="200px">
          Log In
        </Button>
      </Flex>
    </>
  );
};
