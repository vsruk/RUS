import { HomeHeader } from "@/components/features/HomeHeader/HomeHeader";
import { Box, Flex, Text } from "@chakra-ui/react";

export default function Home() {
  return (
    <>
      <Flex direction="column" height="100vh">
        <Box width="100%" height="15%">
          <HomeHeader />
        </Box>
      </Flex>
    </>
  );
}
