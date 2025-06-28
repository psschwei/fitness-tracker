import { BrowserRouter as Router, Routes, Route } from 'react-router-dom'
import Layout from './components/Layout/Layout'
import Journal from './pages/Journal'
import Charts from './pages/Charts'

function App() {
  return (
    <Router>
      <Layout>
        <Routes>
          <Route path="/" element={<Journal />} />
          <Route path="/charts" element={<Charts />} />
        </Routes>
      </Layout>
    </Router>
  )
}

export default App 