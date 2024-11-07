import { CreateForm } from "@/components/features/CreateForm/CreateForm";
import { Flex } from "@chakra-ui/react";

export default function CreatePage() {
  return (
    <Flex margin="auto" width="50%" marginTop="50px">
      <CreateForm />
    </Flex>
  );
}
